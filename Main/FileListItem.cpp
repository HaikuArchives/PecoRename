/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <NodeInfo.h>
#include <Node.h>
#include <Entry.h>
#include <strstream.h>
#include <ColumnTypes.h>

#include "constants.h"
#include "functions.h"

#include "FileListItem.h"

FileListItem::FileListItem(const char *name, int64 size, time_t timer, const entry_ref *ref) : BRow(){
	

	fGroesse 	= size;
	fZeit		= timer;

	BEntry		myEntry = BEntry(ref, true);
	if (myEntry.InitCheck() != B_OK) myEntry = BEntry(ref, false);
	BNode 		myNode = BNode(&myEntry);
	BNodeInfo	myNodeInfo;
	if (myNode.InitCheck() == B_OK) myNodeInfo.SetTo(&myNode);
	
	fIcon		= new BBitmap(BRect(0,0,15,15), B_RGBA32 );
	myNodeInfo.GetTrackerIcon(fIcon, B_MINI_ICON);
	
	myNodeInfo.GetType((char *)&fMimeType);

	SetName(name);

	fErrorStatus = 0;

	int32 i = 0;
	SetField(new BBitmapField(fIcon), i++);
	SetField(new BStringField(fName), i++);
	SetField(new BSizeField(size), i++);
	SetField(new BDateField(&timer), i++);
	SetField(new BStringField(fNewName), i++);

}

FileListItem::~FileListItem() {
	// TODO delete fIcon;
}

void FileListItem::SetNewName( BString myNewName ) {
	if (myNewName == fName)
		fNewName = "";
	else
		fNewName = myNewName;

	SetField(new BStringField(fNewName), 4);
}

void FileListItem::SetName( BString name ) {
	fName = name;
	SetField(new BStringField(fName), 1);
}

bool FileListItem::CompareWith(FileListItem *CompareItem) {
	fErrorStatus=0;
	if ( CompareItem==this ) return false;
	if ( fNewName == "" ) return true;
	
	BString	*CompareString;
	if (CompareItem->fNewName != "") CompareString = &CompareItem->fNewName; else CompareString = &CompareItem->fName;
	
	if (fNewName == *CompareString) { fErrorStatus=1; CompareItem->fErrorStatus=1; return true; } else return false;

};
