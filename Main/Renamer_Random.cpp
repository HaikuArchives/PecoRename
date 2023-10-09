/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017 - 2023, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */


#include <Catalog.h>
#include <LayoutBuilder.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <SeparatorView.h>
#include <TextControl.h>
#include <UTF8.h>

#include <string>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"
#include "Renamer_Random.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_InsertOverwrite"


Renamer_Random::Renamer_Random()
	:
	Renamer()
{
	fName = B_TRANSLATE("Random symbols");

	BPopUpMenu* myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Insert"),
		new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Overwrite with"),
		new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fInsertOrOverwrite = new BMenuField(NULL, myMenu);

	fCount = new BSpinner(NULL, B_TRANSLATE("Count:"), new BMessage(MSG_RENAME_SETTINGS));
	fCount->SetMinValue(1);
	fCount->SetValue(5);

	fUpperCase = new BCheckBox(
		"uppercase", B_TRANSLATE("Upper case"), new BMessage(MSG_RENAME_SETTINGS));
	fUpperCase->SetValue(B_CONTROL_ON);

	fLowerCase = new BCheckBox(
		"lowercase", B_TRANSLATE("Lower case"), new BMessage(MSG_RENAME_SETTINGS));
	fLowerCase->SetValue(B_CONTROL_ON);

	fNumbers = new BCheckBox(
		"numbers", B_TRANSLATE("Numbers"), new BMessage(MSG_RENAME_SETTINGS));
	fNumbers->SetValue(B_CONTROL_ON);

	fSpecialSymbols = new BCheckBox(
		"specialsymbols", B_TRANSLATE("Symbols"), new BMessage(MSG_RENAME_SETTINGS));
	fSpecialSymbols->SetToolTip(spec_symbols);

	fCustom = new BCheckBox(
		"custom", B_TRANSLATE("Custom:"), new BMessage(MSG_CUSTOM_SYMBOLS));
	fCustom->SetToolTip(B_TRANSLATE("Custom set of characters"));

	fCustomInput
		= new BTextControl("custominput", "", NULL, new BMessage(MSG_RENAME_SETTINGS));
	fCustomInput->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));
	fCustomInput->SetDivider(0);
	fCustomInput->SetEnabled(false);

	fPosition = new BSpinner(NULL, B_TRANSLATE("At position:"),
		new BMessage(MSG_RENAME_SETTINGS));
	fPosition->SetMinValue(0);
	fPosition->SetValue(0);

	myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the front (left)"),
		new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("from the back (right)"),
		new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fDirection = new BMenuField(NULL, NULL, myMenu);

	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_VERTICAL)
			.AddGroup(B_HORIZONTAL)
				.Add(fInsertOrOverwrite)
				.Add(fCount)
			.End()
			.AddGroup(B_HORIZONTAL)
				.Add(fPosition)
				.Add(fDirection)
			.End()
		.End()
		.Add(new BSeparatorView(B_VERTICAL))
		.AddGrid(B_USE_DEFAULT_SPACING, 0, 1)
			.Add(fUpperCase, 0, 0)
			.Add(fLowerCase, 0, 1)
			.Add(fNumbers, 1, 0)
			.Add(fSpecialSymbols, 1, 1)
			.Add(fCustom, 0, 2)
			.Add(fCustomInput, 1, 2)
		.End();
}


void
Renamer_Random::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_CUSTOM_SYMBOLS:
		{
			if (fCustom->Value() == B_CONTROL_ON)
				fCustomInput->SetEnabled(true);
			else
				fCustomInput->SetEnabled(false);
		} break;
		default:
			Renamer::MessageReceived(message);
	}
}


void
Renamer_Random::RenameList(BList* FileList)
{
	Renamer::RenameList(FileList);

	bool Overwrite = bool(fInsertOrOverwrite->Menu()
		->IndexOf(fInsertOrOverwrite->Menu()->FindMarked()));

	bool FromRight = bool(fDirection->Menu()
		->IndexOf(fDirection->Menu()->FindMarked()));

	FileListItem* ListItem;
	BString ResultString, Part2;
	int EndPart1, StartPart2;
	int32 UTF_LengthOfInsert, LengthOfInsert;

	std::string symbols = "";
	if (fUpperCase->Value() == B_CONTROL_ON)
		symbols += en_upsymbols;
	if (fLowerCase->Value() == B_CONTROL_ON)
		symbols += en_lowsymbols;
	if (fNumbers->Value() == B_CONTROL_ON)
		symbols += num_symbols;
	if (fSpecialSymbols->Value() == B_CONTROL_ON)
		symbols += spec_symbols;
	if (fCustom->Value() == B_CONTROL_ON)
		symbols += fCustomInput->Text();

	if (symbols == "")
		return;

	const char* allSymbols = symbols.c_str();
	int32 length = fCount->Value();

	UTF_LengthOfInsert = LengthOfInsert = length;
	char* tempInsertStr = new char[UTF_LengthOfInsert + 1];

	int32 Position = fPosition->Value();
	int32 positionMaxValue = 0;
	int32 UTF_LengthOfFilename, LengthOfFilename;

	for (int i = 0; i < fNumberOfItems; i++) {
		ListItem = (FileListItem*)FileList->ItemAt(i);
		UTF_LengthOfFilename = LengthOfFilename = ListItem->fName.Length();

		if (LengthOfFilename > positionMaxValue)
			positionMaxValue = LengthOfFilename;

		char* tempStr = new char[UTF_LengthOfFilename + 1];

		char* randomstring = new char[length];
		srand(real_time_clock_usecs() + real_time_clock());
		for (int i = 0; i < length; i++)
			randomstring[i] = allSymbols[rand() % strlen(allSymbols)];
		randomstring[length] = '\0';

		convert_from_utf8(B_ISO1_CONVERSION, randomstring, &UTF_LengthOfInsert,
			tempInsertStr, &LengthOfInsert, 0);
		tempInsertStr[LengthOfInsert] = 0;

		convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(),
			&UTF_LengthOfFilename, tempStr, &LengthOfFilename, 0);
		tempStr[LengthOfFilename] = 0;

		if (FromRight)
			EndPart1 = (LengthOfFilename >= Position) ? LengthOfFilename - Position : 0;
		else
			EndPart1 = (LengthOfFilename >= Position) ? Position : LengthOfFilename;

		ResultString.SetTo(tempStr, EndPart1);

		if (Overwrite) {
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
		char* utf_String = new char[UTF_LengthOfFilename + 1];

		convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(),
			&LengthOfFilename, utf_String, &UTF_LengthOfFilename, 0);
		utf_String[UTF_LengthOfFilename] = 0;

		ListItem->SetNewName(utf_String);
	}
	fPosition->SetMaxValue(positionMaxValue);
}


void
Renamer_Random::DetachedFromWindow()
{
	BMessage msg;
	BMenu* menu = fInsertOrOverwrite->Menu();
	msg.AddBool("overwrite", bool(menu->IndexOf(menu->FindMarked())));
	msg.AddInt32("count", fCount->Value());
	msg.AddBool("uppercase", fUpperCase->Value() == B_CONTROL_ON);
	msg.AddBool("lowercase", fLowerCase->Value() == B_CONTROL_ON);
	msg.AddBool("numbers", fNumbers->Value() == B_CONTROL_ON);
	msg.AddBool("specialsymbols", fSpecialSymbols->Value() == B_CONTROL_ON);
	msg.AddBool("custom", fCustom->Value() == B_CONTROL_ON);
	msg.AddString("custominput", fCustomInput->Text());
	msg.AddInt32("position", fPosition->Value());
	menu = fDirection->Menu();
	msg.AddBool("fromright", bool(menu->IndexOf(menu->FindMarked())));

	UpdatePreferences("ren_random", msg);
}


void
Renamer_Random::AttachedToWindow()
{
	fCustom->SetTarget(this);

	BMessage msg;
	ReadPreferences("ren_random", msg);

	BString custom = "";
	int32 integer = 0;
	bool boolean = 0;
	if (msg.FindBool("overwrite", &boolean) == B_OK) {
		BMenu* menu = fInsertOrOverwrite->Menu();
		for (int i = 0; i < 2; ++i)
			menu->ItemAt(i)->SetMarked(i == (int)boolean);
	}
	if (msg.FindInt32("count", &integer) == B_OK)
		fCount->SetValue(integer);
	if (msg.FindBool("uppercase", &boolean) == B_OK)
		fUpperCase->SetValue(boolean ? B_CONTROL_ON : B_CONTROL_OFF);
	if (msg.FindBool("lowercase", &boolean) == B_OK)
		fLowerCase->SetValue(boolean ? B_CONTROL_ON : B_CONTROL_OFF);
	if (msg.FindBool("numbers", &boolean) == B_OK)
		fNumbers->SetValue(boolean ? B_CONTROL_ON : B_CONTROL_OFF);
	if (msg.FindBool("specialsymbols", &boolean) == B_OK)
		fSpecialSymbols->SetValue(boolean ? B_CONTROL_ON : B_CONTROL_OFF);
	if (msg.FindBool("custom", &boolean) == B_OK) {
		fCustom->SetValue(boolean ? B_CONTROL_ON : B_CONTROL_OFF);
		fCustomInput->SetEnabled(boolean);
	}
	BString symbols;
	if (msg.FindString("custominput", &custom) == B_OK)
		fCustomInput->SetText(custom);

	if (msg.FindInt32("position", &integer) == B_OK)
		fPosition->SetValue(integer);
	if (msg.FindBool("fromright", &boolean) == B_OK) {
		BMenu* menu = fDirection->Menu();
		for (int i = 0; i < 2; ++i)
			menu->ItemAt(i)->SetMarked(i == (int)boolean);
	}
}
