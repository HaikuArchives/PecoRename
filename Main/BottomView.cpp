/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "BottomView.h"

#include <Box.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Path.h>
#include <PopUpMenu.h>

#include "constants.h"
#include "functions.h"

BottomView::BottomView(BRect frame) : BBox( frame, "bottomView", B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT) {
	SetLabel(STR_BOTTOMVIEW_TITLE);
}

BottomView::~BottomView() {
	BMenuField	*menuField = (BMenuField *)FindView("selectMode");
	BMenu		*menu = menuField->Menu();

	BMessage	message;
	message.AddInt8("renamer", menu->IndexOf(menuField->Menu()->FindMarked()));
	
	UpdatePreferences("renamer", message);
}

void BottomView::AttachedToWindow() {

	BMessage	message;
	ReadPreferences("renamer", message);

	// --- Pause
	int8	activeRenamer;
	if (message.FindInt8("renamer", &activeRenamer)!=B_OK) activeRenamer = -1;

	BRect frame = Bounds();
	frame.top += be_bold_font->Size();
	frame.InsetBy(7.0, 7.0);

	BPopUpMenu	*myMenu = new BPopUpMenu(STR_PLEASE_SELECT);

	for ( int i = 0; i<MODE_TOTAL; i++ ) {
		BMenuItem *menuitem = new BMenuItem(((PecoApp *)be_app)->fRenamers[i]->fName, new BMessage(MSG_RENAMER));
		if (i==activeRenamer) menuitem->SetMarked(true);
		myMenu->AddItem(menuitem);
	}
	
	BMenuField* SelectButton = new BMenuField( BRect( frame.left, frame.top, frame.right, be_plain_font->Size() ), "selectMode", STR_MODE, myMenu);
	SelectButton->SetDivider(be_plain_font->StringWidth( STR_MODE ) + 8.0 );
	AddChild( SelectButton );

	if (activeRenamer!=-1) be_app->PostMessage(MSG_RENAMER);
}

