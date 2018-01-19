/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017, Janus
 *		2018, Humdinger
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
#include "Renamer_Remove.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_Remove"


Renamer_Remove::Renamer_Remove()
	: Renamer()
{
	fName = B_TRANSLATE("Remove");

	fPosition1 = new BSpinner(NULL,	B_TRANSLATE("Remove from position"),
		new BMessage(MSG_RENAME_SETTINGS));
	fPosition1->SetMinValue(0);
	fPosition1->SetValue(0);

	BPopUpMenu* myMenu;
	myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the front (left)"),
		new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the back (right)"),
		new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fDirection1 = new BMenuField(NULL, NULL, myMenu);

	fPosition2 = new BSpinner(NULL, B_TRANSLATE("To position"),
		new BMessage(MSG_RENAME_SETTINGS));
	fPosition2->SetMinValue(0);
	fPosition2->SetValue(0);

	myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the front (left)"),
		new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the back (right)"),
		new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fDirection2 = new BMenuField(NULL, NULL, myMenu);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(fPosition1)
			.Add(fDirection1)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fPosition2)
			.Add(fDirection2)
		.End()
		.AddGlue();
}


void
Renamer_Remove::RenameList(BList* FileList)
{
	Renamer::RenameList(FileList);

	bool FromRight1 = bool(fDirection1->Menu()
		->IndexOf(fDirection1->Menu()->FindMarked()));
	bool FromRight2 = bool(fDirection2->Menu()
		->IndexOf(fDirection2->Menu()->FindMarked()));

	FileListItem* ListItem;
	BString ResultString, Part2;
	int EndPart1, StartPart2;

	int32 Position1 = fPosition1->Value();
	int32 Position2 = fPosition2->Value();	
	int32 positionMaxValue = 0;
	int32 UTF_LengthOfFilename, LengthOfFilename;
		
	for (int i = 0; i < fNumberOfItems; i++) {
		ListItem = (FileListItem* )FileList->ItemAt(i);
		UTF_LengthOfFilename = LengthOfFilename = ListItem->fName.Length();

		if (LengthOfFilename > positionMaxValue)
			positionMaxValue = LengthOfFilename;

		fPosition1->SetMaxValue(positionMaxValue);
		fPosition2->SetMaxValue(positionMaxValue);

		char* tempStr = new char[UTF_LengthOfFilename + 1];

		convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(),
			&UTF_LengthOfFilename, tempStr, &LengthOfFilename, 0);
		tempStr[LengthOfFilename] = 0;

		if (FromRight1) {
			EndPart1 = (LengthOfFilename >= Position1)
				? LengthOfFilename - Position1 : 0;
		} else {
			EndPart1 = (LengthOfFilename >= Position1)
				? Position1 : LengthOfFilename;
		}

		if (FromRight2) {
			StartPart2 = (LengthOfFilename >= Position2)
				? LengthOfFilename - Position2 : 0;
		} else {
			StartPart2 = (LengthOfFilename >= Position2)
				? Position2 : LengthOfFilename;
		}

		if (StartPart2 < EndPart1)
			StartPart2 = EndPart1;
		
		ResultString.SetTo(tempStr, EndPart1);

		BString(tempStr).CopyInto(Part2, StartPart2,
			LengthOfFilename - StartPart2);
		ResultString.Append(Part2);

		LengthOfFilename = ResultString.Length();
		UTF_LengthOfFilename = LengthOfFilename*  2;
		char* utf_String = new char[UTF_LengthOfFilename + 1];
		
		convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(),
			&LengthOfFilename, utf_String, &UTF_LengthOfFilename, 0);
		utf_String[UTF_LengthOfFilename] = 0;

		ListItem->SetNewName(utf_String);
	}
}


void
Renamer_Remove::DetachedFromWindow()
{
	BMessage msg;
	msg.AddInt32("pos1", fPosition1->Value());
	msg.AddInt32("pos2", fPosition2->Value());

	BMenu* menu = fDirection1->Menu();
	msg.AddBool("direction1", bool(menu->IndexOf(menu->FindMarked())));
	menu = fDirection2->Menu();
	msg.AddBool("direction2", bool(menu->IndexOf(menu->FindMarked())));

	UpdatePreferences("ren_remove", msg);
}


void
Renamer_Remove::AttachedToWindow()
{
	BMessage msg;
	ReadPreferences("ren_remove", msg);
	
	BString string = "";
	int32 integer = 0;
	bool boolean = 0;
	if (msg.FindInt32("pos1", &integer) == B_OK)
		fPosition1->SetValue(integer);
	if (msg.FindInt32("pos2", &integer) == B_OK)
		fPosition2->SetValue(integer);
	if (msg.FindBool("direction1", &boolean) == B_OK) {
		BMenu* menu = fDirection1->Menu();
		for (int i = 0; i < 2; ++i)
			menu->ItemAt(i)->SetMarked(i == (int)boolean);
	}
	if (msg.FindBool("direction2", &boolean) == B_OK) {
		BMenu* menu = fDirection2->Menu();
		for (int i = 0; i < 2; ++i)
			menu->ItemAt(i)->SetMarked(i == (int)boolean);
	}
}
