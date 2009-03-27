/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <Font.h>
#include <Alert.h>
#include <TextControl.h>

#include "constants.h"
#include "functions.h"

#include "TopView.h"
#include "FileView.h"

TopView::TopView(BRect frame) : BBox( frame, "topView", B_FOLLOW_TOP_BOTTOM ) {
	SetLabel(STR_TOPVIEW_TITLE);
};

void TopView::AttachedToWindow() {
	BRect	frame = Bounds();
	frame.top += be_bold_font->Size();
	frame.InsetBy(7.0, 7.0);

	BButton* ChooseButton = new BButton( BRect( frame.left, frame.top, frame.left + be_plain_font->StringWidth(STR_PLEASE_CHOOSE) + 20.0, 0.0 ), "selectFiles", STR_PLEASE_CHOOSE, new BMessage( MSG_SELECT_FILES ) );
	AddChild( ChooseButton );
	
	BTextControl* TextControl = new BTextControl( BRect (frame.left + ChooseButton->Frame().Width() + 10.0, frame.top + 2, frame.right  - 16, 0.0), "pfadView", STR_PATH, NULL, NULL, B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW );
	TextControl->SetDivider(be_plain_font->StringWidth(STR_PATH) + 5);
	AddChild( TextControl );

	FileView* aFileView = new FileView( BRect( frame.left, frame.top + be_plain_font->Size() + 20.0, frame.right, frame.bottom) );
	AddChild( aFileView );
}
