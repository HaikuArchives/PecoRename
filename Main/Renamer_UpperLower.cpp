/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Alert.h>
#include <Catalog.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Beep.h>
#include <TextControl.h>
#include <LayoutBuilder.h>

#include "constants.h"

#include "FileListItem.h"
#include "functions.h"
#include "Renamer_UpperLower.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_UpperLower"

Renamer_UpperLower::Renamer_UpperLower() : Renamer() {

	fName 		= B_TRANSLATE("Uppercase / lowercase");

	BPopUpMenu	*myMenu = new BPopUpMenu(B_TRANSLATE("Please select"));

	myMenu->AddItem(new BMenuItem(B_TRANSLATE("UPPERCASE"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("lowercase"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Sentence case"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->AddItem(new BMenuItem(B_TRANSLATE("Title Case"), new BMessage(MSG_RENAME_SETTINGS)));
	myMenu->ItemAt(0)->SetMarked(true);

	fUpperOrLower = new BMenuField(NULL, B_TRANSLATE("Convert to"), myMenu);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(fUpperOrLower)
			.AddGlue()
		.End()
		.AddGlue();
}

void Renamer_UpperLower::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	FileListItem	*ListItem;

	for (int i = 0; i < fNumberOfItems; i++ ) {
		ListItem = (FileListItem *)FileList->ItemAt(i);
		
		int32	laenge = ListItem->fName.Length();
		char	*tempStr = new char[laenge + 1];

		uint8	Buchstabe, Byte0 = 0;
		int32   choice = fUpperOrLower->Menu()->IndexOf(fUpperOrLower->Menu()->FindMarked());
	
		bool isCap = true;
		
		for (int j = 0; j < laenge; j++) {
			Buchstabe = ListItem->fName.ByteAt(j);
			if (Buchstabe>=0xc0) {
				Byte0 = Buchstabe;
			} else {
				switch (choice){
					case 0: if (((Buchstabe>=0x61) && (Buchstabe<=0x7a)) || ( (Byte0==0xc3) && (Buchstabe>=0xa0 && Buchstabe<=0xb6) || (Buchstabe>=0xb8 && Buchstabe<=0xbe)))
						Buchstabe &= ~32;
						break;
					case 1: if (((Buchstabe>=0x41) && (Buchstabe<=0x5a)) || ( (Byte0==0xc3) && (Buchstabe>=0x80 && Buchstabe<=0x96) || (Buchstabe>=0x98 && Buchstabe<=0x9e)))
						Buchstabe |= 32;
						break;
					case 2: 
						// Capitalize only the first letter.
						if (isCap)
							{isCap = false; Buchstabe &= ~32;}
						else {
							if (((Buchstabe>=0x41) && (Buchstabe<=0x5a)) || ( (Byte0==0xc3) && (Buchstabe>=0x80 && Buchstabe<=0x96) || (Buchstabe>=0x98 && Buchstabe<=0x9e)))
								Buchstabe |= 32;
						}
						break;
					case 3:
						if (isCap) {isCap = false; Buchstabe &= ~32;}
						else {
							if (((Buchstabe>=0x41) && (Buchstabe<=0x5a)) || ( (Byte0==0xc3) && (Buchstabe>=0x80 && Buchstabe<=0x96) || (Buchstabe>=0x98 && Buchstabe<=0x9e)))
									Buchstabe |= 32;
						}
						// If space is found, toggle caps again.
						if (Buchstabe==0x20) isCap = true;
						break;
				}
				Byte0 = 0;
			}
			tempStr[j] = Buchstabe;
		}
		tempStr[laenge] = 0;

		ListItem->SetNewName( tempStr );

		delete tempStr;
	}
	
};

void Renamer_UpperLower::DetachedFromWindow() {
	BMessage	msg;

	BMenu *menu = fUpperOrLower->Menu();
	msg.AddBool("upperlower", bool(menu->IndexOf(menu->FindMarked())));
	UpdatePreferences("ren_upperlower", msg);
}

void Renamer_UpperLower::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_upperlower", msg);
	
	bool boolean;
	if (msg.FindBool("upperlower", &boolean)==B_OK) {
		BMenu *menu = fUpperOrLower->Menu();
		for (int i=0; i<2; ++i) menu->ItemAt(i)->SetMarked(i==(int)boolean);
	}

}
