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
#include <Catalog.h>
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

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainView"

MainView::MainView() : BView ("mainView",  B_WILL_DRAW) {

	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	float iconSize = 24;
	BBitmap* folder=new BBitmap(BRect(0,0, iconSize, iconSize), 0, B_RGBA32);
	BMimeType type("application/x-vnd.Be-directory");
	type.GetIcon(folder, B_MINI_ICON);

	BButton* ChooseButton = new BButton("selectFiles", B_TRANSLATE(""),
		new BMessage(MSG_SELECT_FILES));
	ChooseButton->SetIcon(folder);

	BStringView* PathString = new BStringView("pfadView", B_TRANSLATE("Choose files and directories"));
	PathString->SetFont(be_bold_font);
	PathString->SetExplicitMaxSize(BSize(550, 32));
	PathString->SetTruncation(B_TRUNCATE_MIDDLE);

	FileListView* aFileView = new FileListView();

	BScrollBar* scrollBar = (BScrollBar*)aFileView->FindView("horizontal_scroll_bar");

	BGroupLayout* compoundTitle = BLayoutBuilder::Group<>(B_HORIZONTAL, 2)
		.Add(ChooseButton)
		.Add(PathString);

	BGroupLayout *topBox = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(aFileView);

	StatusView* statusView = new StatusView(scrollBar);
	aFileView->AddStatusView(statusView);

	BBox* top = new BBox("top");
	top->SetLabel(B_TRANSLATE("Choose files and directories"));
	top->AddChild(topBox->View());
	top->SetLabel(compoundTitle->View());

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
	fCards->SetVisibleItem((int32)activeRenamer);


	BMenuField* menuField = new BMenuField( "selectMode", "", fRenamers);
	BBox* bottom = new BBox("bottom");
	bottom->SetLabel(menuField);
	bottom->AddChild(cards);

	// StatusBar
	BStatusBar*	statusBar	= new BStatusBar( "statusBar", "", NULL);
	statusBar->Hide();

	// Do it! - Button
	BButton* DoItButton = new BButton( "DoIt", B_TRANSLATE("Rename"), new BMessage(MSG_DO_IT));
	DoItButton->SetEnabled(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(top)
		.Add(bottom)
		.AddGroup(B_HORIZONTAL)
			.Add(statusBar, 100)
			.AddGlue()
			.Add(DoItButton, 0); // TODO how does weigth works?

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

