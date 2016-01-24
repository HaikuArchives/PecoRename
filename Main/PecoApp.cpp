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

#include <Alert.h>
#include <Application.h>
#include <Bitmap.h>
#include <ListView.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <NodeInfo.h>
#include <String.h>
#include <TextControl.h>
#include <FilePanel.h>

#include <malloc.h>
#include <string.h>

#include "constants.h"
#include "functions.h"

#include "About.h"
#include "Fenster.h"
#include "Renamer_SearchReplace.h"
#include "Renamer_Extension.h"
#include "Renamer_Numbering.h"
#include "Renamer_InsertReplace.h"
#include "Renamer_UpperLower.h"
#include "Renamer_Remove.h"

PecoApp::PecoApp() : BApplication("application/x-vnd.pecora-PecoRename") {

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
	fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON, STR_SELECT);
	fFilePanel->SetButtonLabel(B_CANCEL_BUTTON, STR_CANCEL);
	fFilePanel->Window()->SetTitle(STR_PANEL_TITLE);

	fScriptFilePanel = new BFilePanel(B_SAVE_PANEL, NULL, NULL, B_FILE_NODE, false,
		0, 0, true);
	fScriptFilePanel->SetButtonLabel(B_DEFAULT_BUTTON, STR_OK);
	fScriptFilePanel->SetButtonLabel(B_CANCEL_BUTTON, STR_CANCEL);
	fScriptFilePanel->Window()->SetTitle(STR_MENU_CREATE_SCRIPT);

	fWindow->Show();

};

void PecoApp::AboutRequested() {

	const char	*CopyrightTexte[10];
	CopyrightTexte[0] = STR_ABOUT_COPYRIGHT_0;
	CopyrightTexte[1] = STR_ABOUT_COPYRIGHT_1;
	CopyrightTexte[2] = "";
	CopyrightTexte[3] = STR_ABOUT_COPYRIGHT_2;
	CopyrightTexte[4] = NULL;
	
	About(STR_ABOUT_TITLE, STR_APP_NAME, STR_APP_VERSION, CopyrightTexte, STR_ABOUT_THANKYOU, "http://www.pecora.de/pecorename/");
}

bool PecoApp::QuitRequested() {
	return true;
}

void PecoApp::MessageReceived(BMessage *msg) {
	switch( msg->what ) {
		case B_SIMPLE_DATA:			RefsReceived (msg); break;
		case MSG_MENU_NEW:			New(); break;
		case MSG_SELECT_FILES: 		fFilePanel->Show(); break;
		case MSG_DO_IT: 			MakeList(); DoIt(); break;
		case MSG_RENAME_SETTINGS:	MakeList(); break;
		case MSG_MENU_SCRIPT:		if (!NothingToDo()) fScriptFilePanel->Show(); break;
		case B_SAVE_REQUESTED:		CreateScript(msg); break;
		
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
	BTextControl* 	pfadView 	= (BTextControl *)fWindow->FindView("pfadView");
	fWindow->Unlock();
	
	//Pfad finden
	for ( int i=0; msg->FindRef("refs", i, &ref) == B_OK; i++ ) if ( ref.device > 1 ) break;
	
	if ( ref.device > 1 ) {
		New();

		fWindow->Lock();
		((PecoApp *)be_app)->fStatusBar->SetText(STATUS_IMPORT);
		fWindow->Unlock();
	
		aEntry = BEntry(&ref);
		BPath( &aEntry ).GetParent(&fPfad);
				
		fWindow->Lock();
		pfadView->SetText( fPfad.Path() );
		fWindow->Unlock();
		
		//zählen
		type_code	typeFound;
		long		total = 0;
		msg->GetInfo("refs", &typeFound, &total);
		
		fWindow->Lock();
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
					BAlert*	myAlert = new BAlert(NULL, MESSAGE_MULTIDIR, STR_OK);
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

		fStatusBar->Reset(STATUS_STATUS);
		fStatusBar->SetMaxValue(fList->CountItems());
		fWindow->Unlock();
		
		MakeList();
	}
	fWindow->Activate();
	UpdateWindowStatus();
}

void PecoApp::New() {
	
	fWindow->Lock();
	
	fListView->Clear();
	
	BTextControl* 	pfadView = (BTextControl *)fWindow->FindView("pfadView");
	pfadView->SetText(NULL);
	
	fWindow->Unlock();
	
	for( FileListItem* myItem; (myItem = (FileListItem *)fList->RemoveItem((int32)0) ) != NULL; )
		delete myItem;
	
	UpdateWindowStatus();
}

bool PecoApp::NothingToDo() {

	FileListItem	*ListItem;
	
	bool nothing_to_do = true;
	
	for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ )
		if (ListItem->fNewName.Length() > 0 ) { nothing_to_do = false; break; }
	
	if (nothing_to_do) {
		BAlert	*myAlert	= new BAlert(NULL, MESSAGE_NOTHING_TO_DO, STR_WELL);
		myAlert->Go();
	}
	
	return	nothing_to_do;

}

void PecoApp::CreateScript(BMessage *msg) {
	entry_ref	directory;
	const char	*name;
	msg->FindRef("directory", &directory);
	msg->FindString("name", &name);
	
	BPath path(&directory);
	path.Append(name);
	
	BFile file(path.Path(), B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);

	// Initiate
	fWindow->Lock();
	fStatusBar->SetMaxValue(fList->CountItems());
	BTextControl* 	pfadView = (BTextControl *)fWindow->FindView("pfadView");
	BString			Pfad(pfadView->Text());
	fWindow->Unlock();

	FileListItem	*ListItem;
	BString			AlterName, NeuerName;
	BString			output = 
					"#!/bin/sh\n"
					"\n"
					"# This script was created with PecoRename by Werner Freytag.\n"
					"# Visit http://www.pecora.de/pecorename for details!\n\n"
					"alert \"This script will rename files.\n\n"
					"Do you really want to continue?\" \"Yes\" \"No\"\n\n"
					"if [ $? -ne 0 ]\n"
					"then\n"
					"	exit 0\n"
					"fi\n\n";
	
	for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ ) {
		fWindow->Lock();
		fStatusBar->Update(1);
		fWindow->Unlock();
		if (ListItem->fNewName.String() != "" ) {
			AlterName = Pfad; AlterName.Append("/").Append(ListItem->fName);
			AlterName.Replace("\"", "\\\"", AlterName.Length());
			NeuerName = Pfad; NeuerName.Append("/").Append(ListItem->fNewName);
			NeuerName.Replace("\"", "\\\"", NeuerName.Length());
			output << "mv \"" << AlterName.String() << "\" \"" << NeuerName.String() << "\"\n";
		}
	}
	
	file.Write(output.String(), output.Length());
	
	mode_t	perms;
	
	file.GetPermissions(&perms);
	file.SetPermissions(perms | S_IXUSR | S_IXGRP | S_IXOTH );
	
	BNode		node(file);
	BNodeInfo	node_info(&node);
	node_info.SetType("text/plain");
	
	fWindow->Lock();
	fStatusBar->Reset(STATUS_STATUS);
	fWindow->Unlock();

}

void PecoApp::DoIt() {

	if (NothingToDo()) return;
	
	FileListItem	*ListItem;
	
	BAlert	*myAlert	= new BAlert(NULL, MESSAGE_REALLY_DOIT, STR_CONTINUE, STR_CANCEL);
	if (myAlert->Go() == 1) return;
		for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ ) {
		if (ListItem->fErrorStatus == 1 ) {
			BAlert	*myAlert	= new BAlert(NULL, MESSAGE_WILL_HAVE_PROBS, STR_CONTINUE, STR_CANCEL);
			if (myAlert->Go() == 1) return;
			break;
		}
	}

	bool 	noerror = true, nomoreerrors=false, canceled=false;

	fWindow->Lock();
	fStatusBar->SetText(STATUS_RENAMING);
	fStatusBar->SetMaxValue(fList->CountItems());

	BButton		*okButton = (BButton *)fWindow->FindView("DoIt");
	okButton->SetEnabled(false);
	
	BTextControl* 	pfadView = (BTextControl *)fWindow->FindView("pfadView");
	BString	Pfad(pfadView->Text());
	fWindow->Unlock();
	
	BString	AlterName, NeuerName;
	BEntry	Datei;
	
	for (int32 i = 0; (ListItem = (FileListItem *)fListView->ItemAt(i)) != NULL; i++ ) {
		fWindow->Lock();
		fStatusBar->Update(1);
		fWindow->Unlock();
		if (canceled) { ListItem->fErrorStatus=0; continue; }
		if (ListItem->fNewName.String() != "" ) {
			AlterName = Pfad; AlterName.Append("/").Append(ListItem->fName);
			NeuerName = Pfad; NeuerName.Append("/").Append(ListItem->fNewName);
			Datei.SetTo(AlterName.String());
			if ( Datei.Rename(NeuerName.String()) != B_OK ) {
				ListItem->fErrorStatus=1;
				fWindow->Lock();
				fListView->InvalidateItem(i);
				fWindow->Unlock();

				if (!nomoreerrors) {
					noerror = false;

					BString		ErrorMessage(MESSAGE_HAVE_PROBLEM);
					ErrorMessage.ReplaceFirst("%1", ListItem->fName.String());
					ErrorMessage.ReplaceFirst("%2", ListItem->fNewName.String());
					
					BAlert	*myAlert	= new BAlert(NULL, ErrorMessage.String(), STR_CANCEL, STR_CONTINUE, STR_CONTINUE_WO_MSG, B_WIDTH_AS_USUAL, B_WARNING_ALERT);

					int32	result = myAlert->Go();
					if (result == 0) { canceled = true; continue; }
					if (result == 2) nomoreerrors = true;
				}
				
			} else {
			
				fWindow->Lock();
				ListItem->SetName(ListItem->fNewName);
				ListItem->SetNewName("");
				fListView->InvalidateItem(i);
				fWindow->Unlock();

			}
		}
	}

//	NoRenamer();

	fWindow->Lock();
	fStatusBar->Reset(STATUS_STATUS);
	fWindow->Unlock();

	if (noerror) MakeList();
	else {
		fStatusBar->SetText(STATUS_DIDIT_BAD);

		BAlert	*myAlert	= new BAlert(NULL, MESSAGE_MARKED_FILES, STR_OK);
		myAlert->Go();
	}
	
}
