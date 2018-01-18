/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017 - 2018, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */


#include <strstream>

#include <Alert.h>
#include <Beep.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <TextControl.h>
#include <UTF8.h>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"
#include "Renamer_InsertReplace.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_InsertReplace"


Renamer_InsertReplace::Renamer_InsertReplace()
	:
	Renamer()
{
	fName = B_TRANSLATE("Insert / replace");

	BPopUpMenu* myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Insert"),
		new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Replace with"),
		new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fInsertOrReplace = new BMenuField(B_TRANSLATE("Insert or replace:"),
		myMenu);

	fText = new BTextControl( NULL, B_TRANSLATE("Text:"), NULL,
		new BMessage(MSG_RENAME_SETTINGS));
	fText->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));
	fPosition = new BTextControl( NULL, B_TRANSLATE("At position:"), "0",
		new BMessage(MSG_RENAME_SETTINGS));
	fPosition->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));

	myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the front (left)"),
		new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the back (right)"),
		new BMessage(MSG_RENAME_SETTINGS)));

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
			.Add(fDirection)
		.End()
		.AddGlue();
}


void
Renamer_InsertReplace::RenameList(BList* FileList)
{
	Renamer::RenameList(FileList);

	bool Replace = bool(fInsertOrReplace->Menu()
		->IndexOf(fInsertOrReplace->Menu()->FindMarked()));
	BString Text = fText->Text();

	int Position;
	std::strstream iStream, oStream;
	
	do {
		iStream << fPosition->Text(); iStream >> Position;
		if ((Position<0) || (strlen(fPosition->Text()) == 0)) {
			Position = 0;
			oStream << Position; oStream.put(0);
//			fPosition->SetText(oStream.str());
			break;
		}
	} while (false);
	
	bool FromRight = bool(fDirection->Menu()
		->IndexOf(fDirection->Menu()->FindMarked()));

	FileListItem* ListItem;
	BString ResultString, Part2;
	int EndPart1, StartPart2;
	
	int32 UTF_LengthOfInsert, LengthOfInsert;
		
	UTF_LengthOfInsert = LengthOfInsert = Text.Length();
	char	*tempInsertStr = new char[UTF_LengthOfInsert + 1];

	convert_from_utf8(B_ISO1_CONVERSION, Text.String(), &UTF_LengthOfInsert,
		tempInsertStr, &LengthOfInsert, 0);
	tempInsertStr[LengthOfInsert] = 0;

	int32 UTF_LengthOfFilename, LengthOfFilename;
		
	for (int i = 0; i < fNumberOfItems; i++) {
		ListItem = (FileListItem*)FileList->ItemAt(i);
		UTF_LengthOfFilename = LengthOfFilename = ListItem->fName.Length();
		char* tempStr = new char[UTF_LengthOfFilename + 1];

		convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(),
			&UTF_LengthOfFilename, tempStr, &LengthOfFilename, 0);
		tempStr[LengthOfFilename] = 0;

		if (FromRight) {
			EndPart1 = (LengthOfFilename >= Position)
				? LengthOfFilename - Position : 0;
		} else {
			EndPart1 = (LengthOfFilename >= Position)
				? Position : LengthOfFilename;
		}

		ResultString.SetTo(tempStr, EndPart1);

		if (Replace) {
			StartPart2 = (EndPart1 + LengthOfInsert <= LengthOfFilename)
				? EndPart1 + LengthOfInsert : LengthOfFilename;
		} else		
			StartPart2 = EndPart1;
		
		BString(tempStr).CopyInto(Part2, StartPart2,
			LengthOfFilename - StartPart2);
		ResultString.Append(tempInsertStr);
		ResultString.Append(Part2);

		LengthOfFilename = ResultString.Length();
		UTF_LengthOfFilename = LengthOfFilename * 2;
		char	*utf_String = new char[UTF_LengthOfFilename + 1];
		
		convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(),
			&LengthOfFilename, utf_String, &UTF_LengthOfFilename, 0);
		utf_String[UTF_LengthOfFilename] = 0;

		ListItem->SetNewName( utf_String );
	}
};


void
Renamer_InsertReplace::DetachedFromWindow()
{
	BMessage msg;
	BMenu* menu = fInsertOrReplace->Menu();
	msg.AddBool("replace", bool(menu->IndexOf(menu->FindMarked())));
	msg.AddString("text", fText->Text());
	msg.AddString("position", fPosition->Text() );
	menu = fDirection->Menu();
	msg.AddBool("fromright", bool(menu->IndexOf(menu->FindMarked())));
	
	UpdatePreferences("ren_insertreplace", msg);
}


void
Renamer_InsertReplace::AttachedToWindow()
{
	BMessage msg;
	ReadPreferences("ren_insertreplace", msg);
	
	BString string;
	bool boolean;
	if (msg.FindBool("replace", &boolean) == B_OK) {
		BMenu* menu = fInsertOrReplace->Menu();
		for (int i = 0; i<2; ++i)
			menu->ItemAt(i)->SetMarked(i == (int)boolean);
	}
	if (msg.FindString("text", &string) == B_OK)
		fText->SetText(string.String());
	if (msg.FindString("position", &string) == B_OK)
		fPosition->SetText(string.String());
	if (msg.FindBool("fromright", &boolean) == B_OK) {
		BMenu* menu = fDirection->Menu();
		for (int i = 0; i < 2; ++i)
			menu->ItemAt(i)->SetMarked(i == (int)boolean);
	}
}
