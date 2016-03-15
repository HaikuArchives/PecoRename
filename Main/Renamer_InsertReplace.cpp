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
#include "Renamer_InsertReplace.h"

Renamer_InsertReplace::Renamer_InsertReplace() : Renamer() {

	fName 		= REN_INSERTREPLACE;

	BPopUpMenu	*myMenu = new BPopUpMenu(STR_PLEASE_SELECT);
	myMenu->AddItem(new BMenuItem(REN_SET_INSERT, new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(REN_SET_REPLACE, new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fInsertOrReplace = new BMenuField(REN_SET_INSERTREPLACE, myMenu);

	fText = new BTextControl( NULL, REN_SET_TEXT, NULL, new BMessage(MSG_RENAME_SETTINGS));

	fPosition = new BTextControl( NULL, REN_SET_ATPOSITION, "0", new BMessage(MSG_RENAME_SETTINGS));

	myMenu = new BPopUpMenu(STR_PLEASE_SELECT);
	myMenu->AddItem(new BMenuItem(REN_SET_FROMLEFT, new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(REN_SET_FROMRIGHT, new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fDirection = new BMenuField( NULL, NULL, myMenu);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(fInsertOrReplace)
			.Add(fText)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fPosition)
			.Add(fDirection);
}

void Renamer_InsertReplace::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	bool		Replace = bool(fInsertOrReplace->Menu()->IndexOf(fInsertOrReplace->Menu()->FindMarked()));
	BString		Text = fText->Text();

	int			Position;
	strstream	iStream, oStream;
	
	do {
		iStream << fPosition->Text(); iStream >> Position;
		if ((Position<0) || (strlen(fPosition->Text())==0)) {
			Position = 0;
			oStream << Position; oStream.put(0);
//			fPosition->SetText(oStream.str());
			break;
		}
	} while (false);
	
	bool FromRight = bool(fDirection->Menu()->IndexOf(fDirection->Menu()->FindMarked()));

	FileListItem	*ListItem;
	BString			ResultString, Teil2;
	int				EndeTeil1, AnfangTeil2;
	
	int32	UTF_LengthOfInsert, LengthOfInsert;
		
	UTF_LengthOfInsert = LengthOfInsert = Text.Length();
	char	*tempInsertStr = new char[UTF_LengthOfInsert + 1];

	convert_from_utf8(B_ISO1_CONVERSION, Text.String(), &UTF_LengthOfInsert, tempInsertStr, &LengthOfInsert, 0);
	tempInsertStr[LengthOfInsert] = 0;

	int32	UTF_LengthOfFilename, LengthOfFilename;
		
	for (int i = 0; i < fNumberOfItems; i++ ) {
		ListItem = (FileListItem *)FileList->ItemAt(i);
		UTF_LengthOfFilename = LengthOfFilename = ListItem->fName.Length();
		char	*tempStr = new char[UTF_LengthOfFilename + 1];

		convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(), &UTF_LengthOfFilename, tempStr, &LengthOfFilename, 0);
		tempStr[LengthOfFilename] = 0;

		if (FromRight)
			EndeTeil1 = (LengthOfFilename >= Position) ? LengthOfFilename - Position : 0;
		else
			EndeTeil1 = (LengthOfFilename >= Position) ? Position : LengthOfFilename;

		ResultString.SetTo(tempStr, EndeTeil1);

		if (Replace)
			AnfangTeil2 = (EndeTeil1 + LengthOfInsert <= LengthOfFilename) ? EndeTeil1 + LengthOfInsert : LengthOfFilename;
		else			
			AnfangTeil2 = EndeTeil1;
		
		BString(tempStr).CopyInto(Teil2, AnfangTeil2, LengthOfFilename - AnfangTeil2);
		ResultString.Append(tempInsertStr);
		ResultString.Append(Teil2);

		LengthOfFilename = ResultString.Length();
		UTF_LengthOfFilename = LengthOfFilename * 2;
		char	*utf_String = new char[UTF_LengthOfFilename + 1];
		
		convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(), &LengthOfFilename, utf_String, &UTF_LengthOfFilename, 0);
		utf_String[UTF_LengthOfFilename] = 0;

		ListItem->SetNewName( utf_String );
	}
	
};

void Renamer_InsertReplace::DetachedFromWindow() {
	BMessage	msg;
	BMenu *menu = fInsertOrReplace->Menu();
	msg.AddBool("replace", bool(menu->IndexOf(menu->FindMarked())));
	msg.AddString("text", fText->Text());
	msg.AddString("position", fPosition->Text() );
	menu = fDirection->Menu();
	msg.AddBool("fromright", bool(menu->IndexOf(menu->FindMarked())));
	
	UpdatePreferences("ren_insertreplace", msg);
}

void Renamer_InsertReplace::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_insertreplace", msg);
	
	BString string;
	bool boolean;
	if (msg.FindBool("replace", &boolean)==B_OK) {
		BMenu *menu = fInsertOrReplace->Menu();
		for (int i=0; i<2; ++i) menu->ItemAt(i)->SetMarked(i==(int)boolean);
	}
	if (msg.FindString("text", &string)==B_OK)
		fText->SetText(string.String());
	if (msg.FindString("position", &string)==B_OK)
		fPosition->SetText(string.String());
	if (msg.FindBool("fromright", &boolean)==B_OK) {
		BMenu *menu = fDirection->Menu();
		for (int i=0; i<2; ++i) menu->ItemAt(i)->SetMarked(i==(int)boolean);
	}

}
