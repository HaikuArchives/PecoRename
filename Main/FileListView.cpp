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
#include <Catalog.h>
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
#include "PreviewColumn.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "FileListView"


FileListView::FileListView()
	: BColumnListView("fileListView",
		B_FRAME_EVENTS|B_NAVIGABLE) {
	int32 i = 0;
	AddColumn(new BBitmapColumn("Icon", 16, 16, 16, B_ALIGN_CENTER), i++);
	AddColumn(new BStringColumn(B_TRANSLATE("Name"), WIDTH_NAME, 10, 600, 0), i++);
	AddColumn(new BSizeColumn(B_TRANSLATE("Size"), WIDTH_SIZE, 10, 600), i++);
	AddColumn(new BDateColumn(B_TRANSLATE("Modified"), WIDTH_DATE, 10, 600), i++);
	AddColumn(new PreviewColumn(B_TRANSLATE("Preview"), WIDTH_PREVIEW, 10, 600, 0), i++);
}

void FileListView::MouseDown(BPoint where) {
	BColumnListView::MouseDown(where);
//	DeselectAll();
};

bool FileListView::InitiateDrag(BPoint where, bool initialySelected) {
	/*
	FileListItem* Item = (FileListItem*) RowAt(where);
	int32 itemIndex = IndexOf(Item);

	Select(itemIndex);
	//BRect r (0, 0, Item->Width(), Item->Height());

	BRect r (0, 0, 100, Item->Height());
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
	*/

//	DeselectAll();
	return true;
};

void FileListView::KeyDown(const char *bytes, int32 numBytes) {

	FileListItem* Item = (FileListItem*) CurrentSelection();
	if (Item == NULL)
		return;
	int32 selectedItem = IndexOf(Item);
	
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
	return (FileListItem*) RowAt(index);
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
	AddRow(item);
}

void FileListView::AddList(BList* list)
{
	for (int32 i = 0; i < list->CountItems(); i++)
		AddRow((FileListItem*) list->ItemAt(i));
}

void FileListView::RemoveItem(FileListItem* item)
{
	RemoveRow(item);
}

void FileListView::MessageDropped(BMessage* message, BPoint point)
{
	be_app_messenger.SendMessage(message);
}
