/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef FILE_LIST_ITEM_H
#define FILE_LIST_ITEM_H

#include <malloc.h>

#include <ListItem.h>
#include <String.h>
#include <Bitmap.h>
#include <Mime.h>

#include "PecoApp.h"

class FileListItem : public BListItem {
friend	void PecoApp::DoIt();
friend	bool PecoApp::NothingToDo();
friend	void PecoApp::CreateScript(BMessage *msg);
friend	class FileListRow;
public: 
					FileListItem( const char *name, int64 size, time_t timer, const entry_ref *ref ); 
					~FileListItem();
	virtual void	DrawItem(BView *owner, BRect frame, bool complete = false); 
	void			SetNewName( BString	myNewName );
	void 			SetName( BString name );
	bool			CompareWith(FileListItem *CompareItem);
	void			SetRow(FileListRow* row) { fRow = row; }
	FileListRow*	GetRow() const { return fRow; }
	
	BString			fName;
	int64			fGroesse;
	time_t			fZeit;
	char			fMimeType[B_MIME_TYPE_LENGTH];
	
private: 
	char			fErrorStatus;
	BBitmap			*fIcon;
	BString			fListName; 
	BString			fListGroesse;
	BString			fListZeit;
	BString			fNewName;
	BString			fListNewName;
	FileListRow*	fRow;
};

#endif
