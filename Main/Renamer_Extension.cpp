/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Catalog.h>
#include <ControlLook.h>
#include <Mime.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <TextControl.h>
#include <LayoutBuilder.h>

#include "FileListItem.h"
#include "functions.h"
#include "PecoApp.h"
#include "Renamer_Extension.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_Extension"

// Max length for file extensions
#define MAX_EXTENSION_LENGTH 5

Renamer_Extension::Renamer_Extension() : Renamer() {
	fName = B_TRANSLATE("Add file extension");

	fReplaceOldCheckBox = new BCheckBox(NULL,
		B_TRANSLATE("Replace old extension"), new BMessage(MSG_RENAME_SETTINGS));
	fReplaceOldCheckBox->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	fLowerCase = new BRadioButton("lowerCase", B_TRANSLATE("lowercase"),
		new BMessage(MSG_RENAME_SETTINGS));
	fLowerCase->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	fUpperCase = new BRadioButton("upperCase", B_TRANSLATE("UPPERCASE"),
		new BMessage(MSG_RENAME_SETTINGS));
	fUpperCase->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	static const float spacing = be_control_look->DefaultLabelSpacing();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(B_USE_ITEM_INSETS, spacing / 2, B_USE_ITEM_INSETS, 0)
		.Add(fReplaceOldCheckBox)
		.AddStrut(spacing)
		.Add(fLowerCase)
		.Add(fUpperCase)
		.AddGlue();
};

void Renamer_Extension::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	const bool 	replaceold = bool(fReplaceOldCheckBox->Value());
	const int32	upperlower = fLowerCase->Value() == B_CONTROL_ON;

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
					if (upperlower)
						Extension.ToLower();
					else
						Extension.ToUpper();

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
	msg.AddBool("upperlower", (bool)fLowerCase->Value() == B_CONTROL_ON);

	UpdatePreferences("ren_extension", msg);
}

void Renamer_Extension::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_extension", msg);

	bool boolean;
	if (msg.FindBool("replace", &boolean) == B_OK)
		fReplaceOldCheckBox->SetValue(boolean);
	else
		fReplaceOldCheckBox->SetValue(B_CONTROL_OFF); // default

	if (msg.FindBool("upperlower", &boolean) == B_OK) {
		if (boolean)
			fLowerCase->SetValue(B_CONTROL_ON);
		else
			fUpperCase->SetValue(B_CONTROL_ON);
	} else
		fLowerCase->SetValue(B_CONTROL_ON);	// default
}
