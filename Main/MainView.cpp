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

MainView::MainView() : BView ("mainView",  B_FRAME_EVENTS | B_WILL_DRAW) {

	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BButton* ChooseButton = new BButton("selectFiles", B_TRANSLATE("Please select..."),
		new BMessage(MSG_SELECT_FILES));
	BTextControl* PathControl = new BTextControl("pfadView", B_TRANSLATE("Path:"), "", NULL);

	FileListView* aFileView = new FileListView();

	BGroupLayout *topBox = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(ChooseButton)
			.Add(PathControl)
		.End()
		.Add(aFileView);

	BBox* top = new BBox("top");
	top->SetLabel(B_TRANSLATE("Select your files and directories..."));
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

	BStringView* title = new BStringView("", B_TRANSLATE("...and rename them!"));
	title->SetFont(be_bold_font);
	BMenuField* Field = new BMenuField( "selectMode", B_TRANSLATE("Mode:"), fRenamers);

	BGroupLayout* compoundTitle = BLayoutBuilder::Group<>(B_HORIZONTAL)
		.Add(title)
		.Add(Field);

	BBox* bottom = new BBox("bottom");
	bottom->SetLabel(compoundTitle->View());
	bottom->AddChild(cards);

	// StatusBar
	BStatusBar*	statusBar	= new BStatusBar( "statusBar", B_TRANSLATE("Status: "), NULL);
	statusBar->SetText(B_TRANSLATE("Please select files!"));

	// Do it! - Button
	BButton* DoItButton = new BButton( "DoIt", B_TRANSLATE("Let's do it!"), new BMessage(MSG_DO_IT));
	DoItButton->SetEnabled(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(top)
		.Add(bottom)
		.AddGroup(B_HORIZONTAL)
			.Add(statusBar)
			.AddGroup(B_VERTICAL)
				.AddStrut(B_USE_HALF_ITEM_SPACING)
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


void MainView::FrameResized(float height, float width) {
	FindView("fileListView")->Hide();
	BView::FrameResized(height,width);
	FindView("fileListView")->Show();
}

