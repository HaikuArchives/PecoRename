/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Rect.h>
#include <Button.h>
#include <Font.h>
#include <StatusBar.h>
#include <Bitmap.h>
#include <Resources.h>

#include "constants.h"
#include "functions.h"

#include "MainView.h"
#include "TopView.h"
#include "BottomView.h"

MainView::MainView( BRect frame ) : BView (frame, "mainView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW) {

	SetViewColor(216, 216, 216);

	// nutzbare Größe
	frame.OffsetTo( 0.0, 0.0 );
	frame.InsetBy( 5.0, 5.0 );
	
	// Erstellen der beiden inneren Frames
	TopView* topView = new TopView( BRect( frame.left, frame.top, frame.right, frame.bottom - 72.0 - be_plain_font->Size() - be_bold_font->Size() ) );
	AddChild( topView );

	BottomView* bottomView = new BottomView( BRect( frame.left, frame.bottom - 67.0 - be_plain_font->Size() - be_bold_font->Size(), frame.right, frame.bottom - 43.0 ) );
	AddChild( bottomView );

	// StatusBar
	BStatusBar*	statusBar	= new BStatusBar( BRect(frame.left + 5.0, frame.bottom - be_plain_font->Size() - 23.0, frame.left + 400.0, frame.bottom), "statusBar", STATUS_STATUS, NULL);
	statusBar->SetText(STATUS_SELECT_FILES);
	statusBar->SetResizingMode(B_FOLLOW_BOTTOM);
	rgb_color color = { 70, 100, 180, 255};
	statusBar->SetBarColor(color);
	statusBar->SetBarHeight(14.0);
	AddChild(statusBar);
	
	// Do it! - Button
	BButton* DoItButton = new BButton( BRect ( frame.right - be_plain_font->StringWidth(STR_DO_IT) - 25.0, frame.bottom - 30.0, frame.right - 5.0 , frame.bottom - 5.0 ), "DoIt", STR_DO_IT, new BMessage(MSG_DO_IT), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM );
	DoItButton->SetEnabled(false);
	AddChild(DoItButton);
};
