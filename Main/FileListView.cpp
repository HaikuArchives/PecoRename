/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Bitmap.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <String.h>
#include <Window.h>

#include "constants.h"
#include "functions.h"

#include "Fenster.h"
#include "FileListView.h"
#include "FileListItem.h"
#include "PecoApp.h"

FileListView::FileListView(BRect frame)	: BListView(frame, "fileListView", B_SINGLE_SELECTION_LIST, B_FOLLOW_TOP_BOTTOM|B_FRAME_EVENTS, B_WILL_DRAW|B_NAVIGABLE) {
}

void FileListView::MouseDown(BPoint where) {
	BListView::MouseDown(where);
//	DeselectAll();
};

bool FileListView::InitiateDrag(BPoint where, int32 itemIndex, bool initialySelected) {
	FileListItem	*Item = (FileListItem *)ItemAt(itemIndex);

	Select(itemIndex);
	BRect r (0, 0, Item->Width(), Item->Height());

	BBitmap *bmp = new BBitmap(r, B_RGB32, true);
	BView	*ItemView = new BView(r, "",  B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW);

	bmp->Lock();
	bmp->AddChild(ItemView);

	Item->DrawItem(ItemView, r, 1);
	ItemView->Sync();
	bmp->Unlock();
	
	BMessage msg(B_NO_REPLY);
	msg.AddInt32("be:actions", B_TRASH_TARGET);
	DragMessage(&msg, bmp, B_OP_ALPHA, where - BPoint(0, itemIndex * ( Item->Height() + 1)));

	uint32	buttons;
	BPoint	pt;
	do {
		GetMouse( &pt, &buttons);
		snooze(10000);
	} while (buttons);

	if (!Window()->Frame().Contains(ConvertToScreen(pt))) {
		((PecoApp *)be_app)->fWindow->Lock();
		RemoveItem(Item);
		((PecoApp *)be_app)->fList->RemoveItem(Item);
		Invalidate();
		((PecoApp *)be_app)->fWindow->Unlock();
		MakeList();
		delete Item;
	}	

//	DeselectAll();

	return true;
};

void FileListView::KeyDown(const char *bytes, int32 numBytes) {
	int32	selectedItem = CurrentSelection();
	FileListItem	*Item = (FileListItem *)ItemAt(selectedItem);
	
	if ( bytes[0] == 127 ) {
		((PecoApp *)be_app)->fWindow->Lock();
		RemoveItem(Item);
		((PecoApp *)be_app)->fList->RemoveItem(Item);
		((PecoApp *)be_app)->fWindow->Unlock();
		MakeList();
		MakeFocus(true);
		if (selectedItem == CountItems() )
			Select(selectedItem-1);
		else
			Select(selectedItem);
		delete Item;
	} else 
		BListView::KeyDown(bytes, numBytes);
}
