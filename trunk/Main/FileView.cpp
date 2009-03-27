/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Button.h>
#include <Picture.h>
#include <Font.h>
#include <PictureButton.h>
#include <Alert.h>
#include <ScrollBar.h>
#include <ListView.h>

#include "constants.h"
#include "functions.h"

#include "FileView.h"
#include "FileListView.h"

FileView::FileView(BRect frame) : BView( frame, "fileView", B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW ) {
};

void FileView::AttachedToWindow() {
// ** Buttons über der Liste	
	BPictureButton* 	aPictureButton;

	BPoint 	topleft 	= BPoint( 0.0, 0.0 );
	float	height		= be_plain_font->Size() + 5.0;

// Icon
	float	width		= WIDTH_ICON;
	aPictureButton = new BPictureButton( BRect( topleft, topleft + BPoint(width, height) ), NULL,
		new BPicture(), new BPicture(), NULL );
	aPictureButton->SetDisabledOff( CreateButton(width, "", button_flat ) );
	AddChild( aPictureButton );

	aPictureButton->SetEnabled(false);

// Name	
	topleft.x += width; width = WIDTH_NAME;
	aPictureButton = new BPictureButton( BRect( topleft, topleft + BPoint(width, height) ), "Name", 
		CreateButton( width, STR_NAME, button_out ), CreateButton( width, STR_NAME, button_in ), 
		new BMessage( MSG_SORT_BY_NAME), 0, B_FOLLOW_TOP, B_WILL_DRAW );
	aPictureButton->SetDisabledOff ( CreateButton( width, STR_NAME, button_in ) );
	aPictureButton->SetTarget(this);
	AddChild( aPictureButton );

	aPictureButton->SetEnabled(false); fSortButton = aPictureButton;

// Größe
	topleft.x += width; width = WIDTH_SIZE;
	aPictureButton = new BPictureButton( BRect( topleft, topleft + BPoint(width, height) ), "Size",
		CreateButton( width, STR_SIZE, button_out ), CreateButton( width, STR_SIZE, button_in ), 
		new BMessage( MSG_SORT_BY_SIZE ), 0, B_FOLLOW_TOP, B_WILL_DRAW );
	aPictureButton->SetDisabledOff ( CreateButton( width, STR_SIZE, button_in ) );
	aPictureButton->SetTarget(this);
	AddChild( aPictureButton );

// Datum	
	topleft.x += width; width = WIDTH_DATE;
	aPictureButton = new BPictureButton( BRect( topleft, topleft + BPoint(width, height) ), "Date",
		CreateButton( width, STR_DATE, button_out ), CreateButton( width, STR_DATE, button_in ), 
		new BMessage( MSG_SORT_BY_DATE), 0, B_FOLLOW_TOP, B_WILL_DRAW );
	aPictureButton->SetDisabledOff ( CreateButton( width, STR_DATE, button_in ) );
	aPictureButton->SetTarget(this);
	AddChild( aPictureButton );

// Preview	
	topleft.x += width; width = WIDTH_PREVIEW;
	aPictureButton = new BPictureButton( BRect( topleft, topleft + BPoint(width, height) ), NULL,
		new BPicture(), new BPicture(), NULL );
	aPictureButton->SetDisabledOff( CreateButton(width, STR_PREVIEW, button_flat ) );
	AddChild( aPictureButton );

	aPictureButton->SetEnabled(false);
	
	SetFont(be_bold_font);

// ** FileListView
	BRect TextViewFrame = BRect(1.0, be_plain_font->Size() + 6.0, Bounds().Width() - B_V_SCROLL_BAR_WIDTH - 1.0, Bounds().Height() - 1.0);

	FileListView *aListView = new FileListView(TextViewFrame);
	AddChild( aListView );
	
// Scrollbar
	TextViewFrame.left = TextViewFrame.right + 1.0;
	TextViewFrame.right += B_V_SCROLL_BAR_WIDTH + 1.0;
	TextViewFrame.top = 0.0; TextViewFrame.bottom++;

	BScrollBar* ScrollBar = new BScrollBar( TextViewFrame, NULL, aListView, 0.0, 0.0, B_VERTICAL );
	ScrollBar->SetRange(0.0, 0.0);
	AddChild( ScrollBar );
};

BPicture* FileView::CreateButton(float width, const char* text, int mode) {

	BString	Titel = ShortenString(text, width - 8);
	
	BRect	aRect(0.0, 0.0, width , be_plain_font->Size() + 5.0);
	aRect.InsetBy(1,1);
	
	SetFont(be_plain_font);
	font_height	FontHoehe;
	be_plain_font->GetHeight( &FontHoehe );

	BeginPicture(new BPicture);

		switch (mode) {
			case button_in: 	SetLowColor(60, 80, 150); break;
			default: 			SetLowColor(70, 100, 180);
		}
		
		FillRect(aRect, B_SOLID_LOW );
		
		if (mode != button_flat) {
			SetPenSize(1.0);

			switch (mode) {
				case button_in:	SetHighColor(30, 45, 90); break;
				default: 		SetHighColor(100, 130, 255); 
			}
	
			MovePenTo( aRect.RightTop() );
			StrokeLine( aRect.LeftTop() );
			StrokeLine( aRect.LeftBottom() );
	
			switch (mode) {
				case button_in:	SetHighColor(72, 96, 190); break;
				default: 		SetHighColor(40, 70, 120); 
			}
	
			StrokeLine( aRect.RightBottom() );
			StrokeLine( aRect.RightTop() );
	
		}
		aRect.InsetBy(-1, -1);
		
		MovePenTo( ( aRect.Width() - StringWidth( Titel.String() ) ) / 2, ( aRect.Height() - FontHoehe.descent + FontHoehe.ascent ) / 2);

		SetHighColor(255, 255, 255);
		DrawString(Titel.String());
		
		SetHighColor(100, 100, 100);
		StrokeRect( aRect );

		SetHighColor(0, 0, 0);

		SetFont(be_plain_font);

	return EndPicture();
}

void FileView::MessageReceived(BMessage *msg) {
	switch (msg->what) {
		case MSG_SORT_BY_NAME:
		case MSG_SORT_BY_SIZE:
		case MSG_SORT_BY_DATE:
		{
			fSortButton->SetEnabled(true);
			switch (msg->what) {
				case MSG_SORT_BY_NAME: {
					fSortButton = (BPictureButton*)FindView("Name");
					}; break;
				case MSG_SORT_BY_SIZE: {
					fSortButton = (BPictureButton*)FindView("Size");
					}; break;
				case MSG_SORT_BY_DATE: {
					fSortButton = (BPictureButton*)FindView("Date");
					}; break;
			}
			fSortButton->SetEnabled(false);
			MakeList();
		} break;
		default:
			be_app->PostMessage(msg);
	}
};

void FileView::Draw(BRect frame) {

	BView* aListView = FindView("fileListView");
	
	BRect aRect = aListView->Frame().InsetByCopy( -1.0, -1.0);
	
	SetHighColor(150, 150, 150);
	FillRect( aRect );
}
