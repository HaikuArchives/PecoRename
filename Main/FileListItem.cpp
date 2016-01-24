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

FileListItem::FileListItem(const char *name, int64 size, time_t timer, const entry_ref *ref) : BListItem(){ 
	
	fRow		= NULL;
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
	
	// Größe
	strstream	Stream;
	
	if ( size < 0 ) {
	
		Stream << "-";
		Stream.put(0);
		
	} else {

		char*		Einheit;
		float 		floatsize = size;
		
		if ( floatsize > 999 ) { // kB
			Einheit = "kB";
			floatsize = floatsize / 1024;
			if ( floatsize > 9999 ) { // MB
				Einheit = "MB";
				floatsize = floatsize / 1024;
				if ( floatsize > 9999 ) { // GB
					Einheit = "GB";
					floatsize = floatsize / 1024;
					if ( floatsize > 9999 ) { // TB
						Einheit = "TB";
						floatsize = floatsize / 1024;
					}
				}
			}

			int Praezision = 2;
		
			do {
				Stream.seekp(0);
				Stream.setf( ios::fixed );
		
				Stream.precision(Praezision);
		
				Stream << floatsize << " " << Einheit;
				Stream.put(0);
				Praezision--;
			} while ( (Praezision >= 0) && ( be_plain_font->StringWidth( Stream.str() ) > WIDTH_SIZE - 4 ) );

		} else {

			Stream << floatsize << " bytes";
			Stream.put(0);
		}
	}
	
	fListGroesse = ShortenString( BString(Stream.str()), WIDTH_SIZE - 4 );

	// Zeit ausgeben
	tm		*time = localtime(&timer);
	char	buf[100];
	
	// Auf Ausgabegröße anpassen
	strftime((char*)&buf, sizeof(buf) - 1, STR_DATE_TIME_SECS_FORMAT, time);
	if (be_plain_font->StringWidth( (char *)&buf) > WIDTH_DATE - 4 ) {
		strftime((char*)&buf, sizeof(buf) - 1, STR_DATE_TIME_FORMAT, time);
		if (be_plain_font->StringWidth( (char *)&buf) > WIDTH_DATE - 4 ) {
			strftime((char*)&buf, sizeof(buf) - 1, STR_DATE_FORMAT, time);
		}
	}
	
	fListZeit = ShortenString( BString( buf ), WIDTH_DATE - 4 );

	fErrorStatus = 0;
}

FileListItem::~FileListItem() {
	delete fIcon;
}

void FileListItem::DrawItem(BView *owner, BRect frame, bool complete) {

	if (IsSelected())
		owner->SetLowColor(200,220,250, 128);
	else
		owner->SetLowColor(255,255,255, 128);

	owner->FillRect( frame, B_SOLID_LOW );

	owner->SetDrawingMode(B_OP_OVER);
	owner->DrawBitmapAsync(fIcon, BPoint(frame.left + 4, frame.top + (frame.Height() - 16) / 2));
	owner->SetDrawingMode(B_OP_COPY);
	
	float left = WIDTH_ICON;
	
	// Baseline bestimmen
	font_height	myFontHeight;
	be_plain_font->GetHeight(&myFontHeight);

	float	 Baseline = frame.bottom - myFontHeight.descent - ( frame.Height() - be_plain_font->Size() ) / 2 + 1;

	if ( fGroesse == -1) { // Link unterstreichen
		owner->SetHighColor(127, 127, 127);
		owner->StrokeLine(BPoint(left, Baseline + myFontHeight.descent), BPoint(left + be_plain_font->StringWidth(fListName.String()) + 2, Baseline + myFontHeight.descent), B_MIXED_COLORS);
	}

	owner->SetHighColor(0, 0, 0);

	owner->DrawString(fListName.String(), BPoint(left + 2, Baseline ) );

	left += WIDTH_NAME;
	owner->DrawString(fListGroesse.String(), BPoint(left + WIDTH_SIZE - 2 - owner->StringWidth(fListGroesse.String()), Baseline ) );

	left += WIDTH_SIZE;
	owner->DrawString(fListZeit.String(), BPoint( left + WIDTH_DATE - 2 - owner->StringWidth(fListZeit.String()), Baseline ) );

	left += WIDTH_DATE;
	if (fNewName.String() != "") {
		if (fErrorStatus==1) owner->SetHighColor(255, 0, 0); else owner->SetHighColor(20, 50, 150);
		owner->DrawString(fListNewName.String(), BPoint( left + 2, Baseline ) );
	}
}

void FileListItem::SetNewName( BString myNewName ) {
	if (myNewName == fName) fNewName = ""; else fNewName = myNewName;
	fListNewName = ShortenString( fNewName, WIDTH_PREVIEW - 4 );
	if (fRow != NULL)
		fRow->SetField(new BStringField(fListNewName), 4);
}

void FileListItem::SetName( BString name ) {
	fName 		= name;
	fListName	= ShortenString( BString(name), WIDTH_NAME - 4 );
}

bool FileListItem::CompareWith(FileListItem *CompareItem) {
	fErrorStatus=0;
	if ( CompareItem==this ) return false;
	if ( fNewName == "" ) return true;
	
	BString	*CompareString;
	if (CompareItem->fNewName != "") CompareString = &CompareItem->fNewName; else CompareString = &CompareItem->fName;
	
	if (fNewName == *CompareString) { fErrorStatus=1; CompareItem->fErrorStatus=1; return true; } else return false;

};
