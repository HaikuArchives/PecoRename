/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Box.h>

#include "Renamer.h"
#include "FileListItem.h"
#include "constants.h"

Renamer::Renamer() : BView (BRect( 6, be_plain_font->Size() + be_bold_font->Size() + 24, 582, be_bold_font->Size() + 3 * be_plain_font->Size() + 57 ), NULL, B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW) {
	
	fName 		= "";

	SetViewColor(200, 200, 205);
	SetLowColor(200, 200, 205);
};

void Renamer::RenameList(BList *FileList) {
	fNumberOfItems = FileList->CountItems();
	
	FileListItem	*ListItem;
	for (int32 i = 0; i < fNumberOfItems; i++ ) {
		ListItem = (FileListItem *)FileList->ItemAt(i);
		ListItem->SetNewName("");	
	}
};

void Renamer::Draw(BRect updateRect)  {

	BRect	LittleRect = Bounds().InsetByCopy(1.0, 1.0);

	SetHighColor(170, 170, 190);
	MovePenTo(LittleRect.RightTop());
	StrokeLine(LittleRect.LeftTop());
	StrokeLine(LittleRect.LeftBottom());

	SetHighColor(220, 220, 225);
	StrokeLine(LittleRect.RightBottom());
	StrokeLine(LittleRect.RightTop());
	
	SetHighColor(60, 80, 150);
	StrokeRect( Bounds());

}
