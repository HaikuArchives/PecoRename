/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Mime.h>
#include <PopUpMenu.h>
#include <MenuItem.h>

#include "FileListItem.h"
#include "functions.h"
#include "LiveTextControl.h"
#include "PecoApp.h"
#include "Renamer_Extension.h"

// Maximale Länge einer FileExtension
#define MAX_EXTENSION_LENGTH 5

Renamer_Extension::Renamer_Extension() : Renamer() {
	fName 		= REN_EXTENSION;

	BRect	frame = Bounds().InsetByCopy(4.0, 4.0);
	frame = BRect(4.0, 4.0, frame.Width() - 8.0, be_plain_font->Size() + 10);

	fReplaceOldCheckBox = new BCheckBox(frame, NULL, REN_SET_EXT_REPLACE_OLD, new BMessage(MSG_RENAME_SETTINGS));
	AddChild(fReplaceOldCheckBox);

	frame.OffsetBy (0, be_plain_font->Size() + 13);
	BPopUpMenu	*myMenu = new BPopUpMenu(STR_PLEASE_SELECT);
	
	BMenuItem	*MenuItem;
	myMenu->AddItem(MenuItem = new BMenuItem(REN_SET_DEFAULT, new BMessage(MSG_RENAME_SETTINGS)));
	MenuItem->SetMarked(true);
	myMenu->AddItem(new BMenuItem(REN_SET_LOWERCASE, new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(REN_SET_UPPERCASE, new BMessage(MSG_RENAME_SETTINGS)));
	fSelectCaseMenu = new BMenuField( frame, NULL, REN_SET_EXT_UPPERLOWER, myMenu);
	fSelectCaseMenu->SetDivider(be_plain_font->StringWidth(REN_SET_EXT_UPPERLOWER) + 6);
	AddChild(fSelectCaseMenu);

};

void Renamer_Extension::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	const bool 	replaceold = bool(fReplaceOldCheckBox->Value());
	const int32	upperlower = fSelectCaseMenu->Menu()->IndexOf(fSelectCaseMenu->Menu()->FindMarked());

	FileListItem	*ListItem;
	BMessage		msg;
	const char		*ExtensionString;
	BString			Extension, NewName;
	int				OldExtStart;
	
	for (int32 i = 0; i < fNumberOfItems; i++ ) {
		ListItem = (FileListItem *)FileList->ItemAt(i);

		if ( BMimeType((char *)&ListItem->fMimeType).GetFileExtensions(&msg) == B_OK )
			for (int32 j=0; (msg.FindString("extensions", j, &ExtensionString) == B_OK); j++ )
			 	if (strlen(ExtensionString) < MAX_EXTENSION_LENGTH ) {
			 		Extension = ExtensionString; Extension.Prepend(".");
			 		switch (upperlower) {
			 			case 1: Extension.ToLower(); break;
			 			case 2: Extension.ToUpper();
			 		}
					NewName = ListItem->fName;
			 		if ( (replaceold) && ((OldExtStart = NewName.FindLast(".")) > 0) )
						ListItem->SetNewName(NewName.Truncate(OldExtStart).Append(Extension));
					else
						ListItem->SetNewName(NewName.Append(Extension));
					break;
				}
	}

};

void Renamer_Extension::DetachedFromWindow() {
	BMessage	msg;
	msg.AddBool("replace", (bool)fReplaceOldCheckBox->Value() );
	BMenu *menu = fSelectCaseMenu->Menu();
	msg.AddBool("upperlower", bool(menu->IndexOf(menu->FindMarked())));

	UpdatePreferences("ren_extension", msg);
}

void Renamer_Extension::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_extension", msg);
	
	BMenu *menu = fSelectCaseMenu->Menu();
	msg.AddBool("upperlower", bool(menu->IndexOf(menu->FindMarked())));

	BString string;
	bool boolean;
	if (msg.FindBool("replace", &boolean)==B_OK)
		fReplaceOldCheckBox->SetValue(boolean);
	if (msg.FindBool("upperlower", &boolean)==B_OK) {
		BMenu *menu = fSelectCaseMenu->Menu();
		for (int i=0; i<2; ++i) menu->ItemAt(i)->SetMarked(i==(int)boolean);
	}

}
