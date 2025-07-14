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


#include <Alert.h>
#include <Beep.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <TextControl.h>
#include <UnicodeChar.h>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"
#include "Renamer_UpperLower.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_UpperLower"


Renamer_UpperLower::Renamer_UpperLower()
	:
	Renamer()
{
	fName = B_TRANSLATE("Uppercase / lowercase");

	BPopUpMenu* myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));

	myMenu->AddItem(new BMenuItem(B_TRANSLATE("UPPERCASE"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("lowercase"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Sentence case"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Title Case"), new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fUpperOrLower = new BMenuField(NULL, B_TRANSLATE("Convert to:"), myMenu);

	// clang-format off
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(fUpperOrLower)
			.AddGlue()
		.End()
		.AddGlue();
	// clang-format on
}


void
Renamer_UpperLower::RenameList(BList* FileList)
{
	Renamer::RenameList(FileList);

	FileListItem* ListItem;
	{
		char newNameArr[B_FILE_NAME_LENGTH + 4];

		int conversionMenuIndex
			= fUpperOrLower->Menu()->IndexOf(fUpperOrLower->Menu()->FindMarked());

		for (int i = 0; i < fNumberOfItems; i++) {
			ListItem = (FileListItem*)FileList->ItemAt(i);

			// reset for each name the value change during the conversion
			int conversionMode = conversionMenuIndex;

			const char* pointInName = ListItem->fName.String();
			char* pointOutName = newNameArr;
			int32 numChar = ListItem->fName.CountChars();
			uint32 unicodeChar;

			bool titleCase = false;

			for (int32 i = 0; i < numChar; i++) {
				unicodeChar = BUnicodeChar::FromUTF8(&pointInName);

				if (titleCase && BUnicodeChar::IsWhitespace(unicodeChar)) {
					conversionMode = 3; // If in Title Case mode
										// only the first letter is Title case
										// and the others are UpperCase this must
										// be change to 4
				} else {
					switch (conversionMode) {
						case 0: // upperCase
							unicodeChar = BUnicodeChar::ToUpper(unicodeChar);
							break;
						case 1: // lowerCase
							unicodeChar = BUnicodeChar::ToLower(unicodeChar);
							break;
						case 2: // Sentence case
							unicodeChar = BUnicodeChar::ToTitle(unicodeChar);
							conversionMode = 1;
							break;
						case 3: // Title Case
							unicodeChar = BUnicodeChar::ToTitle(unicodeChar);
							conversionMode = 1;
							titleCase = true;
							break;
						case 4: // Title Case with other words in uppercase
							unicodeChar = BUnicodeChar::ToUpper(unicodeChar);
							conversionMode = 1;
							break;
					}
				}

				BUnicodeChar::ToUTF8(unicodeChar, &pointOutName);

				// check for overflow improbable but can happen
				if (pointOutName - newNameArr >= B_FILE_NAME_LENGTH) {
					ListItem->SetError(3);
					break;
				}
			}
			*pointOutName = 0;
			ListItem->SetNewName(newNameArr);
		}
	}
}


void
Renamer_UpperLower::DetachedFromWindow()
{
	BMessage msg;

	BMenu* menu = fUpperOrLower->Menu();
	msg.AddInt32("upperlower", menu->IndexOf(menu->FindMarked()));
	UpdatePreferences("ren_upperlower", msg);
}


void
Renamer_UpperLower::AttachedToWindow()
{
	BMessage msg;
	ReadPreferences("ren_upperlower", msg);

	int32 index;
	if (msg.FindInt32("upperlower", &index) == B_OK) {
		BMenu* menu = fUpperOrLower->Menu();
		menu->ItemAt(index)->SetMarked(true);
	}
}
