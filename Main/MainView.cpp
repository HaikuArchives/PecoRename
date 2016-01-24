/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Alert.h>
#include <Rect.h>
#include <Button.h>
#include <Font.h>
#include <StatusBar.h>
#include <Bitmap.h>
#include <Resources.h>
#include <TextControl.h>
#include <LayoutBuilder.h>
#include <CardView.h>
#include <PopUpMenu.h>
#include <Messenger.h>
#include <Box.h>
#include <StringView.h>

#include "constants.h"
#include "functions.h"

#include "MainView.h"

MainView::MainView() : BView ("mainView", B_FULL_UPDATE_ON_RESIZE) {

	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BButton* ChooseButton = new BButton("selectFiles", STR_PLEASE_CHOOSE,
		new BMessage(MSG_SELECT_FILES));
	BTextControl* PathControl = new BTextControl("pfadView", STR_PATH, "", NULL);

	FileListView* aFileView = new FileListView();

	BGroupLayout *topBox = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(ChooseButton)
			.Add(PathControl)
		.End()
		.Add(aFileView);

	BBox* top = new BBox("top");
	top->SetLabel(STR_TOPVIEW_TITLE);
	top->AddChild(topBox->View());

	BMessage prefs;
	ReadPreferences("renamer", prefs);
	int8 activeRenamer;
	if (prefs.FindInt8("renamer", &activeRenamer) != B_OK)
		activeRenamer = 0;

	fRenamers = new BPopUpMenu("");
	BCardView* cards = new BCardView();
	fCards = cards->CardLayout();

	for ( int i = 0; i<MODE_TOTAL; i++ ) {
		BMenuItem *item = new BMenuItem(((PecoApp *)be_app)->fRenamers[i]->fName,
			new BMessage(MSG_RENAMER));
		if (i == 0 || i == activeRenamer)
			item->SetMarked(true);
		fRenamers->AddItem(item);
		fCards->AddView(((PecoApp *)be_app)->fRenamers[i]);
	}

	((PecoApp *)be_app)->fRenameMode = 0;
	fCards->SetVisibleItem((int32)0);

	BStringView* title = new BStringView("", STR_BOTTOMVIEW_TITLE);
	title->SetFont(be_bold_font);
	BMenuField* Field = new BMenuField( "selectMode", STR_MODE, fRenamers);

	BGroupLayout* compoundTitle = BLayoutBuilder::Group<>(B_HORIZONTAL)
		.Add(title)
		.Add(Field);

	BBox* bottom = new BBox("bottom");
	bottom->SetLabel(compoundTitle->View());
	bottom->AddChild(cards);

	// StatusBar
	BStatusBar*	statusBar	= new BStatusBar( "statusBar", STATUS_STATUS, NULL);
	statusBar->SetText(STATUS_SELECT_FILES);
	rgb_color color = { 70, 100, 180, 255};
	statusBar->SetBarColor(color);

	// Do it! - Button
	BButton* DoItButton = new BButton( "DoIt", STR_DO_IT, new BMessage(MSG_DO_IT));
	DoItButton->SetEnabled(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(top)
		.Add(bottom)
		.AddGroup(B_HORIZONTAL)
			.Add(statusBar)
			.Add(DoItButton);
}


void MainView::MessageReceived(BMessage* message) {

	switch (message->what) {
		case MSG_RENAMER:
		{
			int32 selected = fRenamers->FindMarkedIndex();
			((PecoApp *)be_app)->fRenameMode = selected;
			fCards->SetVisibleItem(selected);
			MakeList();
			break;
		}

		default:
			BView::MessageReceived(message);
			break;
	}
}


void MainView::AttachedToWindow() {
	int32 num = fRenamers->CountItems();
	for (int32 i = 0; i < num; i++) {
		fRenamers->ItemAt(i)->SetTarget(this);
	}
}
