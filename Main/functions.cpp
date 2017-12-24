/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */
#include <Catalog.h>
#include <FindDirectory.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <StopWatch.h>
#include "Fenster.h"

#include "constants.h"
#include "functions.h"
#include "ConsistencyCheck.h"

#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "functions"

#define PREFS_FILENAME "PecoRename_settings"

void MakeList() {
	/*static */
	ConsistencyCheck consistencyCheck;
	BList		*FileList = ((PecoApp *)be_app)->fList;

	//if (consistencyCheck.CountOldNames() == 0) {
	//	consistencyCheck.AddList(FileList);
	//}

	((PecoApp *)be_app)->fWindow->Lock();
	((PecoApp *)be_app)->fRenamers[((PecoApp *)be_app)->fRenameMode]->RenameList(FileList);

	{
		BStopWatch stopWatch("CheckDup");
		consistencyCheck.AddList(FileList);
		consistencyCheck.ResetNewName();
		consistencyCheck.AddNewList(FileList);
		consistencyCheck.CheckForDuplicates();
		consistencyCheck.PrintStatistic();
	}

	((PecoApp *)be_app)->fWindow->Unlock();
	UpdateWindowStatus();
}


void UpdateWindowStatus() {
	BButton		*okButton = (BButton *)((PecoApp *)be_app)->fWindow->FindView("DoIt");
	((PecoApp *)be_app)->fWindow->Lock();

	BMenuItem	*scriptMenu = (BMenuItem *)((PecoApp *)be_app)->fWindow->KeyMenuBar()->FindItem(B_TRANSLATE("Create shell script..."));

	bool isenabled = !((PecoApp *)be_app)->NothingToDo();

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
