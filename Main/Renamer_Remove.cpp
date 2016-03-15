/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <strstream.h>

#include <Alert.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Beep.h>
#include <UTF8.h>
#include <TextControl.h>
#include <LayoutBuilder.h>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"
#include "Renamer_Remove.h"

Renamer_Remove::Renamer_Remove() : Renamer() {

	fName 		= REN_REMOVE;

	fPosition1 = new BTextControl( NULL, REN_SET_FROMPOS, "0", new BMessage(MSG_RENAME_SETTINGS));

	BPopUpMenu	*myMenu;
	myMenu = new BPopUpMenu(STR_PLEASE_SELECT);
	myMenu->AddItem(new BMenuItem(REN_SET_FROMLEFT, new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(REN_SET_FROMRIGHT, new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fDirection1 = new BMenuField( NULL, NULL, myMenu);

	fPosition2 = new BTextControl( NULL, REN_SET_TOPOS, "0", new BMessage(MSG_RENAME_SETTINGS));

	myMenu = new BPopUpMenu(STR_PLEASE_SELECT);
	myMenu->AddItem(new BMenuItem(REN_SET_FROMLEFT, new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(REN_SET_FROMRIGHT, new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fDirection2 = new BMenuField( NULL, NULL, myMenu);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(fPosition1)
			.Add(fDirection1)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fPosition2)
			.Add(fDirection2);
}

void Renamer_Remove::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	int			Position1, Position2;
	strstream	iStream, oStream;
	
	do {
		iStream << fPosition1->Text(); iStream >> Position1;
		if ((Position1<0) || (strlen(fPosition1->Text())==0)) {
			Position1 = 0;
			oStream << Position1; oStream.put(0);
//			fPosition1->SetText(oStream.str());
			break;
		}
	} while (false);

	do {
		iStream << fPosition2->Text(); iStream >> Position2;
		if ((Position2<0) || (strlen(fPosition2->Text())==0)) {
			Position2 = 0;
			oStream << Position2; oStream.put(0);
//			fPosition2->SetText(oStream.str());
			break;
		}
	} while (false);

	bool FromRight1 = bool(fDirection1->Menu()->IndexOf(fDirection1->Menu()->FindMarked()));
	bool FromRight2 = bool(fDirection2->Menu()->IndexOf(fDirection2->Menu()->FindMarked()));

	
	FileListItem	*ListItem;
	BString			ResultString, Teil2;
	int				EndeTeil1, AnfangTeil2;
	
	int32	UTF_LengthOfFilename, LengthOfFilename;
		
	for (int i = 0; i < fNumberOfItems; i++ ) {
		ListItem = (FileListItem *)FileList->ItemAt(i);
		UTF_LengthOfFilename = LengthOfFilename = ListItem->fName.Length();
		char	*tempStr = new char[UTF_LengthOfFilename + 1];

		convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(), &UTF_LengthOfFilename, tempStr, &LengthOfFilename, 0);
		tempStr[LengthOfFilename] = 0;

		if (FromRight1)
			EndeTeil1 = (LengthOfFilename >= Position1) ? LengthOfFilename - Position1 : 0;
		else
			EndeTeil1 = (LengthOfFilename >= Position1) ? Position1 : LengthOfFilename;

		if (FromRight2)
			AnfangTeil2 = (LengthOfFilename >= Position2) ? LengthOfFilename - Position2 : 0;
		else
			AnfangTeil2 = (LengthOfFilename >= Position2) ? Position2 : LengthOfFilename;
		
		if (AnfangTeil2<EndeTeil1) AnfangTeil2 = EndeTeil1;
		
		ResultString.SetTo(tempStr, EndeTeil1);

		BString(tempStr).CopyInto(Teil2, AnfangTeil2, LengthOfFilename - AnfangTeil2);
		ResultString.Append(Teil2);

		LengthOfFilename = ResultString.Length();
		UTF_LengthOfFilename = LengthOfFilename * 2;
		char	*utf_String = new char[UTF_LengthOfFilename + 1];
		
		convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(), &LengthOfFilename, utf_String, &UTF_LengthOfFilename, 0);
		utf_String[UTF_LengthOfFilename] = 0;

		ListItem->SetNewName( utf_String );
	
	}
};

void Renamer_Remove::DetachedFromWindow() {
	BMessage	msg;

	msg.AddString("pos1", fPosition1->Text());
	msg.AddString("pos2", fPosition2->Text());
	BMenu *menu = fDirection1->Menu();
	msg.AddBool("direction1", bool(menu->IndexOf(menu->FindMarked())));
	menu = fDirection2->Menu();
	msg.AddBool("direction2", bool(menu->IndexOf(menu->FindMarked())));

	UpdatePreferences("ren_remove", msg);
}

void Renamer_Remove::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_remove", msg);
	
	BString string;
	bool boolean;
	if (msg.FindString("pos1", &string)==B_OK)
		fPosition1->SetText(string.String());
	if (msg.FindString("pos2", &string)==B_OK)
		fPosition2->SetText(string.String());
	if (msg.FindBool("direction1", &boolean)==B_OK) {
		BMenu *menu = fDirection1->Menu();
		for (int i=0; i<2; ++i) menu->ItemAt(i)->SetMarked(i==(int)boolean);
	}
	if (msg.FindBool("direction2", &boolean)==B_OK) {
		BMenu *menu = fDirection2->Menu();
		for (int i=0; i<2; ++i) menu->ItemAt(i)->SetMarked(i==(int)boolean);
	}

}
