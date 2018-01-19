/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel, Hannah
 *		2017 - 2018, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
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

#include "MainWindow.h"
#include "Renamer_Extension.h"
#include "Renamer_InsertReplace.h"
#include "Renamer_Numbering.h"
#include "Renamer_Remove.h"
#include "Renamer_SearchReplace.h"
#include "Renamer_UpperLower.h"
#include "ReportWindow.h"

static const char kAppsignature[] = "application/x-vnd.pecora-PecoRename";

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PecoApp"


PecoApp::PecoApp()
	:
	BApplication(kAppsignature)
{
	fRenameMode	= 0;
	fPath = NULL;
	fList = new BList();

	fRenamers[0] = new Renamer_SearchReplace();
	fRenamers[1] = new Renamer_InsertReplace();
	fRenamers[2] = new Renamer_Remove();
	fRenamers[3] = new Renamer_Numbering();
	fRenamers[4] = new Renamer_Extension();
	fRenamers[5] = new Renamer_UpperLower();
};


void
PecoApp::ReadyToRun()
{
	BMessage msg;
	ReadPreferences("main_window", msg);

	BRect rect;
	if (msg.FindRect("pos", &rect) != B_OK)
		rect = BRect(20, 40, 640, 460);		// default

	fWindow = new MainWindow(rect);
	fListView = (FileListView*)fWindow->FindView("fileListView");
	fStatusBar = (BStatusBar*)fWindow->FindView("statusBar");

	UpdateWindowStatus();

	fFilePanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL,
		B_FILE_NODE | B_DIRECTORY_NODE);
	fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON, B_TRANSLATE("Select"));
	fFilePanel->SetButtonLabel(B_CANCEL_BUTTON, B_TRANSLATE("Cancel"));
	fFilePanel->Window()->SetTitle(B_TRANSLATE("Select files for renaming"));

	fWindow->Show();
};


void PecoApp::AboutRequested()
{
	BAboutWindow* about = new BAboutWindow(
		B_TRANSLATE_SYSTEM_NAME("PecoRename"), kAppsignature);
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


bool
PecoApp::QuitRequested()
{
	return true;
}


void
PecoApp::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case B_SIMPLE_DATA:
			RefsReceived (msg);
			break;
		case MSG_MENU_NEW:
			New();
			break;
		case MSG_SELECT_FILES:
			fFilePanel->Show();
			break;
		case MSG_DO_IT:
			DoIt();
			break; // Use the current List
		case MSG_RENAME_SETTINGS:
			MakeList();
			break;

		default:
			BApplication::MessageReceived(msg);
	}
}


void
PecoApp::RefsReceived (BMessage* msg)
{
	entry_ref ref;
	BPath aPath;
	BEntry aEntry;
	off_t size;
	time_t timer;
	
	fWindow->Lock();
	BStringView* pathView = (BStringView*)fWindow->FindView("pathView");
	fWindow->Unlock();
	
	// Find path
	for (int i = 0; msg->FindRef("refs", i, &ref) == B_OK; i++)
		if (ref.device > 1)
			break;

	BVolume volume(ref.device);
	if (volume.IsReadOnly()) {
		BAlert* myAlert = new BAlert(NULL, B_TRANSLATE(
			"The volume is read only: files cannot be renamed."),
			B_TRANSLATE("OK"));
		myAlert->Go();
		return;
	}

	fWindow->Lock();
	BButton* ChooseButton = (BButton*) fWindow->FindView("selectFiles");
	ChooseButton->SetFlat(true);
	fWindow->Unlock();

	if (ref.device > 1) {
		New();

		aEntry = BEntry(&ref);
		BPath(&aEntry).GetParent(&fPath);
				
		fWindow->Lock();
		pathView->SetText(fPath.Path());
		fWindow->Unlock();
		
		// count
		type_code typeFound;
		int32 total = 0;
		msg->GetInfo("refs", &typeFound, &total);
		
		fWindow->Lock();
		fStatusBar->SetText("");
		fStatusBar->Show();
		fStatusBar->SetMaxValue(total);
		fWindow->Unlock();
		
		BPath newPath;
		bool didntshow_msgmultidir = true;
		
		for (int i = 0; msg->FindRef("refs", i, &ref) == B_OK; i++) {
			fWindow->Lock();
			fStatusBar->Update(1);
			fWindow->Unlock();
			
			// filter out devices
			if (ref.device == 1) continue;
 			
			// filter out files with wrong path
			aEntry = BEntry(&ref);
			aPath = BPath(&aEntry);
			
			BPath(&aEntry).GetParent(&newPath);
			
			if ((strcmp(fPath.Path(), newPath.Path()) != 0)) {
				if (didntshow_msgmultidir) {
					BAlert*	myAlert = new BAlert(NULL, B_TRANSLATE(
						"Files from different folders cannot be renamed.\n\n"
						"Only the files of the first found folder will be "
						"imported!"), B_TRANSLATE("OK"));
					myAlert->Go();
					didntshow_msgmultidir = false;
				}
				continue;
			}
			
			// Read values
			if (aEntry.IsFile())
				aEntry.GetSize(&size); 
			else if (aEntry.IsSymLink())
				size = -1;
			else if (aEntry.IsDirectory())
				size = -2;
			else
				continue;
			
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


void
PecoApp::New()
{	
	fWindow->Lock();	
	fListView->Clear();
	
	BStringView* pathView = (BStringView*)fWindow->FindView("pathView");
	pathView->SetText(NULL);
	
	fWindow->Unlock();
	
	fList->MakeEmpty(); // TODO This can be removed, it's redundant of fListView
	
	UpdateWindowStatus();
}


bool
PecoApp::NothingToDo()
{
	FileListItem* ListItem;
	bool nothing_to_do = true;

	for (int32 i = 0; (ListItem = (FileListItem*)fListView->ItemAt(i))
			!= NULL; i++)
		if (ListItem->fNewName.Length() > 0) {
			nothing_to_do = false;
			break;
		}

	return nothing_to_do;
}


void
PecoApp::DoIt()
{
	if (NothingToDo())
		return;

	fWindow->Lock();
	fStatusBar->SetText("");
	fStatusBar->Show();
	fStatusBar->SetMaxValue(fList->CountItems());

	BButton* okButton = (BButton*)fWindow->FindView("DoIt");
	okButton->SetEnabled(false);
	
	BStringView* pathView = (BStringView*)fWindow->FindView("pathView");
	BString	Path(pathView->Text());
	fWindow->Unlock();
	
	BString	OldName, NewName;
	BEntry	File;
	FileListItem* ListItem;

	for (int32 i = 0; (ListItem = (FileListItem*)fListView->ItemAt(i))
			!= NULL; i++) {
		fWindow->Lock();
		fStatusBar->Update(1);
		fWindow->Unlock();
		if (ListItem->fNewName != "" && ListItem->Error() == 0) {
			OldName = Path; OldName.Append("/").Append(ListItem->fName);
			NewName = Path; NewName.Append("/").Append(ListItem->fNewName);
			File.SetTo(OldName.String());
			status_t result;
			if ((result = File.Rename(NewName.String())) != B_OK) {
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

	for (int32 i = 0; (ListItem = (FileListItem*)fListView->ItemAt(i))
			!= NULL; i++) {
		if (ListItem->fErrorStatus > 1) {
			noerror = false;
			break;
		} else if (ListItem->fErrorStatus > 0)
			noerrorDup = false;
	}

	if (noerror && noerrorDup)
		MakeList();
	else {
		if (!noerror) {
			BMessage msg;
			ReadPreferences("report_window", msg);

			BRect rect;
			if (msg.FindRect("size", &rect) != B_OK)
				rect = BRect(60, 80, 705, 280);		// default size

			rect.OffsetBy(fWindow->Frame().LeftTop() + BPoint(80, 100));
			
			ReportWindow* reportWindow = new ReportWindow(rect, fList);
			reportWindow->Show();
		}
	}
}
