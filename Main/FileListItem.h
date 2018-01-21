/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2014, diversys
 *		2016, Hannah
 *		2017, Janus
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef FILE_LIST_ITEM_H
#define FILE_LIST_ITEM_H

#include <malloc.h>

#include <Bitmap.h>
#include <Mime.h>
#include <String.h>

#include "PecoApp.h"

class FileListItem : public BRow {
	friend void 	PecoApp::DoIt();
	friend bool 	PecoApp::NothingToDo();

public: 
					FileListItem(const char* name, int64 size, time_t timer,
						const entry_ref* ref);
					~FileListItem();

			bool	CompareWith(FileListItem* CompareItem);
			void 	SetName(BString name);
			void	SetNewName(BString myNewName);

		BString		fName;
			char	fMimeType[B_MIME_TYPE_LENGTH];
		BString		fNewName;
			void	SetError(char err);
			char	Error() { return fErrorStatus; };

private: 
			char	fErrorStatus;
		BBitmap*	fIcon;
};

#endif // FILE_LIST_ITEM_H
