/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <FindDirectory.h>
#include <MenuBar.h>
#include <MenuItem.h>

#include "Fenster.h"

#include "constants.h"
#include "functions.h"

#define PREFS_FILENAME "PecoRename_settings"

void MakeList() {
	BList		*FileList = ((PecoApp *)be_app)->fList;

	((PecoApp *)be_app)->fWindow->Lock();
	((PecoApp *)be_app)->fStatusBar->SetText(B_TRANSLATE("Sorting..."));
// Keep the code below until sortings by BColumnListView is fully tested.
#if 0
	do {
		if (strcmp(SortButton->Name(), "Name") == 0) {
			((PecoApp *)be_app)->fListView->SortItems(SortByName); FileList->SortItems(SortByName); break; }
		if (strcmp(SortButton->Name(), "Date") == 0) {
			((PecoApp *)be_app)->fListView->SortItems(SortByDate); FileList->SortItems(SortByDate); break; }
		((PecoApp *)be_app)->fListView->SortItems(SortBySize); FileList->SortItems(SortBySize);
	} while (false);
#endif
	((PecoApp *)be_app)->fStatusBar->SetText(B_TRANSLATE("Creating preview..."));

	((PecoApp *)be_app)->fRenamers[((PecoApp *)be_app)->fRenameMode]->RenameList(FileList);

	// Auf Duplikate überprüfen und markieren
	((PecoApp *)be_app)->fStatusBar->SetText(B_TRANSLATE("Are there any problems?"));
	FileListItem	*ListItem;

	((PecoApp *)be_app)->fStatusBar->Reset(B_TRANSLATE("Status: "));

	((PecoApp *)be_app)->fWindow->Unlock();
	UpdateWindowStatus();
}


void UpdateWindowStatus() {
	BButton		*okButton = (BButton *)((PecoApp *)be_app)->fWindow->FindView("DoIt");
	((PecoApp *)be_app)->fWindow->Lock();

	BMenuItem	*scriptMenu = (BMenuItem *)((PecoApp *)be_app)->fWindow->KeyMenuBar()->FindItem(B_TRANSLATE("Create shell script..."));

	bool isenabled = false; // Is Ok-Button enabled?

	do {
		if (((PecoApp *)be_app)->fList->IsEmpty()) {
			((PecoApp *)be_app)->fStatusBar->SetText(B_TRANSLATE("Please select files!"));
			break;
		}
		// else:
		isenabled = true;
		((PecoApp *)be_app)->fStatusBar->SetText(B_TRANSLATE("Awaiting your instructions... :-)"));
	} while (false);

	okButton->SetEnabled(isenabled);
	scriptMenu->SetEnabled(isenabled);

	((PecoApp *)be_app)->fWindow->Unlock();

}

BFile PrefsFile( int32 mode ) {
	BPath	path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.SetTo( path.Path(), PREFS_FILENAME );

	return BFile( path.Path(), mode );
}

void UpdatePreferences( const char *identifier, BMessage &msg ) {

	BMessage	message;
	BFile file = PrefsFile( B_READ_ONLY );
	message.Unflatten( &file );

	message.RemoveName(identifier);
	message.AddMessage(identifier, &msg);

	file = PrefsFile( B_WRITE_ONLY|B_CREATE_FILE );
	file.SetSize( 0 );
	message.Flatten( &file );
}

void ReadPreferences( const char *identifier, BMessage &msg ) {

	BMessage	message;
	BFile file = PrefsFile( B_READ_ONLY );
	message.Unflatten( &file );

	message.FindMessage(identifier, &msg);
}
