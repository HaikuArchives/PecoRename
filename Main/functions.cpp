/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2011, Axel Dörfler
 *		2016, Markus Himmel, Hannah
 *		2017 - 2018, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */


#include "MainWindow.h"
#include <Catalog.h>
#include <FindDirectory.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <StringFormat.h>

#include "constants.h"
#include "functions.h"

#include "ConsistencyCheck.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "functions"

#define PREFS_FILENAME "PecoRename_settings"


void
MakeList()
{
	BList numberingList;
	BList* FileList = ((PecoApp*)be_app)->fList;
	// When numbering the items it's important the order choose by the user
	if (((PecoApp*)be_app)->fRenameMode == 3) {
		FileListView* sortedList = ((PecoApp*)be_app)->fListView;
		for (int32 i = 0; i < sortedList->CountRows(); i++)
			numberingList.AddItem((FileListItem*)sortedList->ItemAt(i));
		FileList = &numberingList;
	}
	((PecoApp*)be_app)->fWindow->Lock();
	((PecoApp*)be_app)->fRenamers[((PecoApp*)be_app)->fRenameMode]->RenameList(FileList);

	ConsistencyCheck(FileList).CheckForDuplicates();

	if (((PecoApp*)be_app)->fRenameMode == 3)
		numberingList.MakeEmpty();

	((PecoApp*)be_app)->fWindow->Unlock();
	UpdateWindowStatus();
}


void
UpdateWindowStatus()
{
	BButton* okButton = (BButton*)((PecoApp*)be_app)->fWindow->FindView("DoIt");
	StatusView* statusView = (StatusView*)((PecoApp*)be_app)->fWindow->FindView("statusview");
	FileListView* fileListView
		= (FileListView*)((PecoApp*)be_app)->fWindow->FindView("fileListView");
	((PecoApp*)be_app)->fWindow->Lock();

	bool isenabled = !((PecoApp*)be_app)->NothingToDo();

	okButton->SetEnabled(isenabled);

	BString itemsNumber;
	BString renamesNumber;
	BString duplicatesNumber;

	BList* fileList = ((PecoApp*)be_app)->fList;
	static BStringFormat formatItems(B_TRANSLATE("{0, plural,"
												 "=0{no items}"
												 "=1{1 item}"
												 "other{# items}}"));
	formatItems.Format(itemsNumber, fileList->CountItems());

	int duplicates = 0;
	int renames = 0;

	for (int32 i = 0; i < fileList->CountItems(); i++) {
		if (((FileListItem*)fileList->ItemAt(i))->fNewName != "") {
			renames++;
			if (((FileListItem*)fileList->ItemAt(i))->Error() != 0)
				duplicates++;
		}
	}
	if (renames > 0) {
		static BStringFormat formatRenamed(B_TRANSLATE("{0, plural,"
													   "=0{no renames}"
													   "=1{1 rename}"
													   "other{# renames}}"));
		formatRenamed.Format(renamesNumber, renames);
	}
	if (duplicates > 0) {
		static BStringFormat formatDuplicated(B_TRANSLATE("{0, plural,"
														  "=0{no duplicates}"
														  "=1{1 duplicate}"
														  "other{# duplicates}}"));
		formatDuplicated.Format(duplicatesNumber, duplicates);
	}
	statusView->Update(itemsNumber, renamesNumber, duplicatesNumber);

	((PecoApp*)be_app)->fWindow->Unlock();
}


BFile
PrefsFile(int32 mode)
{
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.SetTo(path.Path(), PREFS_FILENAME);

	return BFile(path.Path(), mode);
}


void
UpdatePreferences(const char* identifier, BMessage& msg)
{
	BMessage message;
	BFile file = PrefsFile(B_READ_ONLY);
	message.Unflatten(&file);

	message.RemoveName(identifier);
	message.AddMessage(identifier, &msg);

	file = PrefsFile(B_WRITE_ONLY | B_CREATE_FILE);
	file.SetSize(0);
	message.Flatten(&file);
}


void
ReadPreferences(const char* identifier, BMessage& msg)
{
	BMessage message;
	BFile file = PrefsFile(B_READ_ONLY);
	message.Unflatten(&file);

	message.FindMessage(identifier, &msg);
}
