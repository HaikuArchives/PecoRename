/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <strstream>

#include <Alert.h>
#include <Catalog.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Beep.h>
#include <TextControl.h>
#include <LayoutBuilder.h>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"
#include "Renamer_Numbering.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_Numbering"

Renamer_Numbering::Renamer_Numbering() : Renamer() {

	char *Modi[] = { "1, 2, 3, " B_UTF8_ELLIPSIS, "01, 02, 03, " B_UTF8_ELLIPSIS, "001, 002, 003, " B_UTF8_ELLIPSIS, "0001, 0002, 0003, " B_UTF8_ELLIPSIS};

	fName 		= B_TRANSLATE("Numbering");

	BPopUpMenu	*myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));
	for ( uint32 i = 0; i < sizeof(Modi) / sizeof(char *); i++ )
		myMenu->AddItem(new BMenuItem(Modi[i], new BMessage(MSG_RENAME_SETTINGS)));

	myMenu->ItemAt(0)->SetMarked(true);

	fFormat = new BMenuField( NULL, B_TRANSLATE("Format:"), myMenu);

	fStartWith = new BTextControl( NULL, B_TRANSLATE("Start with:"), "0", new BMessage(MSG_RENAME_SETTINGS));
	fStartWith->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));

	fTextBefore = new BTextControl( NULL, B_TRANSLATE("Text before:"), NULL, new BMessage(MSG_RENAME_SETTINGS));
	fTextBefore->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));
	fTextBehind = new BTextControl( NULL, B_TRANSLATE("Text behind:"), NULL, new BMessage(MSG_RENAME_SETTINGS));
	fTextBehind->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));

	BLayoutBuilder::Grid<>(this)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddMenuField(fFormat, 0, 0)
		.AddTextControl(fTextBefore, 0, 1)
		.AddTextControl(fStartWith, 2, 0)
		.AddTextControl(fTextBehind, 2, 1)
		.AddGlue(0, 2);
}

void Renamer_Numbering::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	int			Startzahl;
	std::strstream	iStream, oStream;
	
	if (strlen(fStartWith->Text())==0) {
		Startzahl = 0;
		oStream << Startzahl; oStream.put(0);
		fStartWith->SetText(oStream.str());
	} else {
		iStream << fStartWith->Text(); iStream >> Startzahl;
		if ((Startzahl<0)) {
			Startzahl = 0;
			oStream << Startzahl; oStream.put(0);
//			fStartWith->SetText(oStream.str());
		}
	}
	
	int MinAnzStellen = fFormat->Menu()->IndexOf(fFormat->Menu()->FindMarked()) + 1;

	BString TextBefore = fTextBefore->Text();
	BString TextBehind = fTextBehind->Text();
	
	FileListItem	*ListItem;
	BString			Nummer;
		
	for (int64 i = 0; i < fNumberOfItems; i++ ) {
		
		ListItem = (FileListItem *)FileList->ItemAt(i);

		std::strstream 	oStream;
		BString		NummerString;
		
		oStream << i + Startzahl; oStream.put(0);
		NummerString = oStream.str();

		if (NummerString.Length() < MinAnzStellen)
			NummerString.Prepend(BString("000").Truncate(MinAnzStellen - NummerString.Length()));
		
		BString		ResultString = TextBefore;
		ResultString.Append(NummerString);
		ResultString.Append(TextBehind);
		ListItem->SetNewName( ResultString );
	}
};

void Renamer_Numbering::DetachedFromWindow() {
	BMessage	msg;

	msg.AddString("start_with", fStartWith->Text());
	BMenu *menu = fFormat->Menu();
	msg.AddInt8("positions", menu->IndexOf(menu->FindMarked()));
	msg.AddString("text_before", fTextBefore->Text());
	msg.AddString("text_behind", fTextBehind->Text());
	
	UpdatePreferences("ren_numbering", msg);
}

void Renamer_Numbering::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_numbering", msg);
	
	BString string;
	int8 number;

	if (msg.FindString("start_with", &string)==B_OK)
		fStartWith->SetText(string.String());

	if (msg.FindInt8("positions", &number)==B_OK) {
		BMenu *menu = fFormat->Menu();
		for (int i=0; i<4; ++i) menu->ItemAt(i)->SetMarked(i==number);
	}
	if (msg.FindString("text_before", &string)==B_OK)
		fTextBefore->SetText(string.String());
	if (msg.FindString("text_behind", &string)==B_OK)
		fTextBehind->SetText(string.String());

}
