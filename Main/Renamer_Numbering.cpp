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

#include <stdio.h>

#include <Alert.h>
#include <Beep.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <TextControl.h>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"
#include "Renamer_Numbering.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_Numbering"

Renamer_Numbering::Renamer_Numbering()
	:
	Renamer()
{
	const char* Modi[] = {
		"1, 2, 3, " B_UTF8_ELLIPSIS,
		"01, 02, 03, " B_UTF8_ELLIPSIS,
		"001, 002, 003, " B_UTF8_ELLIPSIS,
		"0001, 0002, 0003, " B_UTF8_ELLIPSIS,
		"00001, 00002, 00003, " B_UTF8_ELLIPSIS
	};

	fName = B_TRANSLATE("Numbering");

	BPopUpMenu* myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	for (uint32 i = 0; i < sizeof(Modi) / sizeof(char*); i++)
		myMenu->AddItem(new BMenuItem(Modi[i],
			new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fFormat = new BMenuField( NULL, B_TRANSLATE("Format:"), myMenu);

	fStartWith = new BSpinner( NULL, B_TRANSLATE("Start with:"),
		new BMessage(MSG_RENAME_SETTINGS));
	fStartWith->SetMinValue(0);
	fStartWith->SetValue(0);

	fTextBefore = new BTextControl( NULL, B_TRANSLATE("Text before:"), NULL,
		new BMessage(MSG_RENAME_SETTINGS));
	fTextBefore->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));
	fTextBehind = new BTextControl( NULL, B_TRANSLATE("Text after:"), NULL,
		new BMessage(MSG_RENAME_SETTINGS));
	fTextBehind->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));

	BLayoutBuilder::Grid<>(this)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddMenuField(fFormat, 0, 0)
		.AddTextControl(fTextBefore, 0, 1)
		.Add(fStartWith->CreateLabelLayoutItem(), 2, 0, 1, 1)
		.Add(fStartWith->CreateTextViewLayoutItem(), 3, 0, 1, 1)
		.AddTextControl(fTextBehind, 2, 1)
		.AddGlue(0, 4);
}


void
Renamer_Numbering::RenameList(BList* FileList)
{
	Renamer::RenameList(FileList);

	int32 StartNumber = fStartWith->Value();
	
	int MinDigits = fFormat->Menu()
		->IndexOf(fFormat->Menu()->FindMarked()) + 1;

	BString TextBefore = fTextBefore->Text();
	BString TextBehind = fTextBehind->Text();
	
	FileListItem* ListItem;
	BString	Nummer;
		
	for (int64 i = 0; i < fNumberOfItems; i++) {
		ListItem = (FileListItem*)FileList->ItemAt(i);

		BString NumberString;
		char start[B_PATH_NAME_LENGTH + 1];
		snprintf(start, sizeof(start), "%" B_PRId32, StartNumber + i);
		NumberString.Append(start);

		if (NumberString.Length() < MinDigits)
			NumberString.Prepend(BString("000").Truncate(MinDigits
				- NumberString.Length()));
		
		BString ResultString = TextBefore;
		ResultString.Append(NumberString);
		ResultString.Append(TextBehind);
		ListItem->SetNewName(ResultString);
	}
}


void
Renamer_Numbering::DetachedFromWindow()
{
	BMessage msg;

	msg.AddInt32("start_with", fStartWith->Value());
	BMenu* menu = fFormat->Menu();
	msg.AddInt8("positions", menu->IndexOf(menu->FindMarked()));
	msg.AddString("text_before", fTextBefore->Text());
	msg.AddString("text_behind", fTextBehind->Text());
	
	UpdatePreferences("ren_numbering", msg);
}


void
Renamer_Numbering::AttachedToWindow()
{
	BMessage msg;
	ReadPreferences("ren_numbering", msg);

	BString string;
	int32 start;
	int8 number;

	if (msg.FindInt32("start_with", &start) == B_OK)
		fStartWith->SetValue(start);

	if (msg.FindInt8("positions", &number) == B_OK) {
		BMenu* menu = fFormat->Menu();
		for (int i = 0; i < 4; ++i)
			menu->ItemAt(i)->SetMarked(i == number);
	}
	if (msg.FindString("text_before", &string) == B_OK)
		fTextBefore->SetText(string.String());
	if (msg.FindString("text_behind", &string) == B_OK)
		fTextBehind->SetText(string.String());
}
