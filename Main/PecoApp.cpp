/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "PecoApp.h"

#include <AboutWindow.h>
#include <Alert.h>
#include <Application.h>
#include <Bitmap.h>
#include <Catalog.h>
#include <FilePanel.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <NodeInfo.h>
#include <String.h>
#include <StringView.h>
#include <TextControl.h>
#include <Volume.h>

#include <malloc.h>
#include <string.h>

#include "constants.h"
#include "functions.h"
#include "Fenster.h"
#include "Renamer_SearchReplace.h"
#include "Renamer_Extension.h"
#include "Renamer_Numbering.h"
#include "Renamer_InsertReplace.h"
#include "Renamer_UpperLower.h"
#include "Renamer_Remove.h"
#include "ReportWindow.h"

static const char kAppSignature[] = "application/x-vnd.pecora-PecoRename";

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PecoApp"

PecoApp::PecoApp() : BApplication(kAppSignature) {

	fRenameMode	= 0;
	fPfad		= NULL;
	fList		= new BList();

	fRenamers[0] = new Renamer_SearchReplace();
	fRenamers[1] = new Renamer_InsertReplace();
	fRenamers[2] = new Renamer_Remove();
	fRenamers[3] = new Renamer_Numbering();
	fRenamers[4] = new Renamer_Extension();
	fRenamers[5] = new Renamer_UpperLower();
};

void PecoApp::ReadyToRun() {

	fWindow = new Fenster();
	fListView	= (FileListView *)fWindow->FindView("fileListView");
	fStatusBar	= (BStatusBar *)fWindow->FindView("statusBar");

	UpdateWindowStatus();

	fFilePanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL, B_FILE_NODE|B_DIRECTORY_NODE);
	fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON, B_TRANSLATE("Select"));
	fFilePanel->SetButtonLabel(B_CANCEL_BUTTON, B_TRANSLATE("Cancel"));
	fFilePanel->Window()->SetTitle(B_TRANSLATE("Select files for renaming"));

	fWindow->Show();
};

void PecoApp::AboutRequested()
{
	BAboutWindow* about = new BAboutWindow(B_TRANSLATE_SYSTEM_NAME("PecoRename"),
		kAppSignature);
	const char* extraCopyrights[] = {
		"2011 Axel Dörfler",
		"2014 Diver",
		"2016 Markus Himmel, Hannahyp",
		"2017-2018 Janus, Humdinger",
		NULL
	};
	const char* authors[] = {
		B_TRANSLATE("Werner Freytag (original author)"),
		"Axel Dörfler",
		"Diver",
		"Hannahyp",
		"Humdinger",
		"Janus",
		"Markus Himmel",
		NULL
	};
	about->AddCopyright(2000, "Werner Freytag", extraCopyrights);
	about->AddAuthors(authors);
	about->Show();
}



bool PecoApp::QuitRequested() {
	return true;
}

void PecoApp::MessageReceived(BMessage *msg) {
	switch( msg->what ) {
		case B_SIMPLE_DATA:			RefsReceived (msg); break;
		case MSG_MENU_NEW:			New(); break;
		case MSG_SELECT_FILES: 		fFilePanel->Show(); break;
		case MSG_DO_IT: 			DoIt(); break; // Use the current List
		case MSG_RENAME_SETTINGS:	MakeList(); break;

	   	default:
	   		BApplication::MessageReceived(msg);
	}
};

void PecoApp::RefsReceived ( BMessage* msg ) {

	entry_ref	ref;
	BPath		aPath;
	BEntry		aEntry;
	off_t		size;
	time_t		timer;
	
	fWindow->Lock();
	BStringView* 	pfadView 	= (BStringView *)fWindow->FindView("pfadView");
	fWindow->Unlock();
	
	//Pfad finden
	for ( int i=0; msg->FindRef("refs", i, &ref) == B_OK; i++ ) if ( ref.device > 1 ) break;

	BVolume volume(ref.device);
	if (volume.IsReadOnly()) {
	   BAlert  *myAlert = new BAlert(NULL, B_TRANSLATE("The volume is read only you cannot rename the files"), B_TRANSLATE("OK"));
	   myAlert->Go();
	   return;
	}

	fWindow->Lock();
	BButton* ChooseButton = (BButton *) fWindow->FindView("selectFiles");
	ChooseButton->SetFlat(true);
	fWindow->Unlock();

	if ( ref.device > 1 ) {
		New();

		aEntry = BEntry(&ref);
		BPath( &aEntry ).GetParent(&fPfad);
				
		fWindow->Lock();
		pfadView->SetText( fPfad.Path() );
		fWindow->Unlock();
		
		//zählen
		type_code	typeFound;
		int32		total = 0;
		msg->GetInfo("refs", &typeFound, &total);
		
		fWindow->Lock();
		fStatusBar->SetText("");
		fStatusBar->Show();
		fStatusBar->SetMaxValue( total );
		fWindow->Unlock();
		
		BPath	newPath;
		
		bool	didntshow_msgmultidir = true;
		
		for ( int i=0; msg->FindRef("refs", i, &ref) == B_OK; i++ ) {
			
			fWindow->Lock();
			fStatusBar->Update(1);
			fWindow->Unlock();
			
			// Laufwerke ausfiltern
			if ( ref.device == 1 ) continue;
 			
			// Dateien mit falschem Pfad ausfiltern
			aEntry = BEntry(&ref);
			aPath = BPath(&aEntry);
			
			BPath( &aEntry ).GetParent(&newPath);
			
			if ( (strcmp( fPfad.Path(), newPath.Path() ) != 0 ) ) {
				if ( didntshow_msgmultidir ) {
					BAlert*	myAlert = new BAlert(NULL, B_TRANSLATE("Files from different directories cannot be renamed.\n\nOnly the files in the first found directory will be imported!"), B_TRANSLATE("Ok"));
					myAlert->Go();
					didntshow_msgmultidir = false;
				}
				continue;
			}
			
			// Werte auslesen
			if (aEntry.IsFile()) aEntry.GetSize(&size); 
			else
				if (aEntry.IsSymLink()) size = -1;
				else
					if (aEntry.IsDirectory()) size = -2;
					else continue;
			
			aEntry.GetModificationTime(&timer);
			fList->AddItem(new FileListItem(aPath.Leaf(), size, timer, &ref));
			
		}
		
		fWindow->Lock();
		fListView->AddList(fList);
		fStatusBar->Hide();
		fStatusBar->SetMaxValue(fList->CountItems());
		fStatusBar->Reset("");
		fWindow->Unlock();
		
		MakeList();
	}
	fWindow->Activate();
	UpdateWindowStatus();
}

void PecoApp::New() {
	
	fWindow->Lock();
	
	fListView->Clear();
	
	BStringView* 	pfadView = (BStringView *)fWindow->FindView("pfadView");
	pfadView->SetText(NULL);
	
	fWindow->Unlock();
	
	fList->MakeEmpty(); // TODO This can be remove is redundance of fListView
	
	UpdateWindowStatus();
}

bool PecoApp::NothingToDo() {

	FileListItem	*ListItem;
	
	bool nothing_to_do = true;

	for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ )
		if (ListItem->fNewName.Length() > 0 ) { nothing_to_do = false; break; }

	return	nothing_to_do;

}

void PecoApp::DoIt() {

	if (NothingToDo()) return;

	fWindow->Lock();
	fStatusBar->SetText("");
	fStatusBar->Show();
	fStatusBar->SetMaxValue(fList->CountItems());

	BButton		*okButton = (BButton *)fWindow->FindView("DoIt");
	okButton->SetEnabled(false);
	
	BStringView* 	pfadView = (BStringView *)fWindow->FindView("pfadView");
	BString	Pfad(pfadView->Text());
	fWindow->Unlock();
	
	BString	AlterName, NeuerName;
	BEntry	Datei;
	FileListItem    *ListItem;
	for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ ) {
		fWindow->Lock();
		fStatusBar->Update(1);
		fWindow->Unlock();
		if (ListItem->fNewName != "" && ListItem->Error() == 0) {
			AlterName = Pfad; AlterName.Append("/").Append(ListItem->fName);
			NeuerName = Pfad; NeuerName.Append("/").Append(ListItem->fNewName);
			Datei.SetTo(AlterName.String());
			status_t result;
			if ( (result = Datei.Rename(NeuerName.String())) != B_OK ) {
				if (result == B_FILE_EXISTS)
					ListItem->SetError(2);
				else
					ListItem->SetError(3);
				fWindow->Lock();
				fListView->InvalidateItem(i);
				fWindow->Unlock();
			} else {
				fWindow->Lock();
				ListItem->SetName(ListItem->fNewName);
				ListItem->SetNewName("");
				fListView->InvalidateItem(i);
				fWindow->Unlock();

			}
		}
	}

	fWindow->Lock();
	fStatusBar->Reset("");
	fStatusBar->Hide();
	fWindow->Unlock();

	bool noerror = true;
	bool noerrorDup = true;

	for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ ) {
		if (ListItem->fErrorStatus > 1) {
				noerror = false;
				break;
		} else if (ListItem->fErrorStatus > 0) {
				noerrorDup = false;
		}
	}

	if (noerror && noerrorDup) MakeList();
	else {
		if (!noerror) {
			ReportWindow *reportWindow = new ReportWindow(fList);
			reportWindow->Show();
		}
	}
	
}
