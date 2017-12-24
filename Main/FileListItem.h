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

#include <String.h>
#include <Bitmap.h>
#include <Mime.h>

#include "PecoApp.h"

class FileListItem : public BRow {
friend	void PecoApp::DoIt();
friend	bool PecoApp::NothingToDo();
friend	void PecoApp::CreateScript(BMessage *msg);
public: 
					FileListItem( const char *name, int64 size, time_t timer, const entry_ref *ref ); 
					~FileListItem();
	void			SetNewName( BString	myNewName );
	void 			SetName( BString name );
	bool			CompareWith(FileListItem *CompareItem);

	BString			fName;
	char			fMimeType[B_MIME_TYPE_LENGTH];
	BString			fNewName;
	void			SetError(char err) {fErrorStatus = err; }
private: 
	char			fErrorStatus;
	BBitmap			*fIcon;


};

#endif
