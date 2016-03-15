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
#include <ColumnTypes.h>
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

FileListRow::FileListRow(FileListItem* item) : BRow(), fItem(item)
{
	int32 i = 0;
	SetField(new BBitmapField(item->fIcon), i++);
	SetField(new BStringField(item->fListName), i++);
	SetField(new BStringField(item->fListGroesse), i++);
	SetField(new BStringField(item->fListZeit), i++);
	SetField(new BStringField(item->fListNewName), i++);
}

FileListView::FileListView()
	: BColumnListView("fileListView",
		B_FRAME_EVENTS|B_NAVIGABLE|B_FULL_UPDATE_ON_RESIZE,
		B_NO_BORDER, false) {
	int32 i = 0;
	AddColumn(new BBitmapColumn(NULL, WIDTH_ICON, 10, 1000), i++);
	AddColumn(new BStringColumn(STR_NAME, WIDTH_NAME, 10, 1000, 0), i++);
	AddColumn(new BStringColumn(STR_SIZE, WIDTH_SIZE, 10, 1000, 0), i++);
	AddColumn(new BStringColumn(STR_DATE, WIDTH_DATE, 10, 1000, 0), i++);
	AddColumn(new BStringColumn(STR_PREVIEW, WIDTH_PREVIEW, 10, 1000, 0), i++);
}

void FileListView::MouseDown(BPoint where) {
	BColumnListView::MouseDown(where);
//	DeselectAll();
};

bool FileListView::InitiateDrag(BPoint where, bool initialySelected) {
	FileListRow* row = (FileListRow*) RowAt(where);
	FileListItem* Item = row->Item();
	int32 itemIndex = IndexOf(row);

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
	FileListRow* row = (FileListRow*) CurrentSelection();
	if (row == NULL)
		return;

	FileListItem* Item = row->Item();
	int32 selectedItem = IndexOf(row);
	
	if ( bytes[0] == 127 ) {
		((PecoApp *)be_app)->fWindow->Lock();
		RemoveItem(Item);
		((PecoApp *)be_app)->fList->RemoveItem(Item);
		((PecoApp *)be_app)->fWindow->Unlock();
		MakeList();
		MakeFocus(true);
		if (selectedItem == CountRows())
			Select(selectedItem-1);
		else
			Select(selectedItem);
		delete Item;
	} else 
		BColumnListView::KeyDown(bytes, numBytes);
}

FileListItem* FileListView::ItemAt(int32 index) const
{
	FileListRow* row = (FileListRow*) RowAt(index);

	return row == NULL ? NULL : row->Item();
}

void FileListView::Select(int32 index)
{
	AddToSelection(RowAt(index));
}

void FileListView::InvalidateItem(int32 index)
{
	InvalidateRow(RowAt(index));
}

void FileListView::AddItem(FileListItem* item)
{
	FileListRow* row = new FileListRow(item);

	AddRow(row);
	item->SetRow(row);
}

void FileListView::AddList(BList* list)
{
	for (int32 i = 0; i < list->CountItems(); i++)
		AddItem((FileListItem*) list->ItemAt(i));
}

void FileListView::RemoveItem(FileListItem* item)
{
	RemoveRow(item->GetRow());
}

void FileListView::MessageDropped(BMessage* message, BPoint point)
{
	be_app_messenger.SendMessage(message);
}
