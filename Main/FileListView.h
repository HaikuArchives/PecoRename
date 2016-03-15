/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef FILE_LIST_VIEW_H
#define FILE_LIST_VIEW_H

#include <ColumnListView.h>

class FileListItem;

class FileListRow : public BRow
{
	FileListItem* fItem;
public:
	FileListRow(FileListItem* item);
	FileListItem* Item() const { return fItem; }
};

class FileListView : public BColumnListView {
public:
					FileListView();
	virtual void	MouseDown(BPoint where);
	virtual	void	KeyDown(const char *bytes, int32 numBytes);
	virtual bool	InitiateDrag(BPoint pt, bool initialySelected);
	FileListItem*	ItemAt(int32 index) const;
			void	Select(int32 index);
			void	InvalidateItem(int32 index);
			void	AddItem(FileListItem* item);
			void	AddList(BList* list);
			void	RemoveItem(FileListItem* item);
			void	MessageDropped(BMessage* message, BPoint point);
};

#endif
