/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017 - 2018, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */


#include <Alert.h>
#include <Bitmap.h>
#include <Box.h>
#include <Button.h>
#include <CardView.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <Font.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <PopUpMenu.h>
#include <Rect.h>
#include <Resources.h>
#include <StatusBar.h>
#include <StringView.h>
#include <TextControl.h>

#include "constants.h"
#include "functions.h"

#include "ContextPopUp.h"
#include "FileListItem.h"
#include "MainView.h"
#include "MainWindow.h"
#include "PecoApp.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainView"


MainView::MainView()
	:
	BView("mainView", B_WILL_DRAW),
	fShowingPopUpMenu(false)
{
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	float iconSize = 24;
	BBitmap* folder = new BBitmap(BRect(0, 0, iconSize, iconSize), 0, B_RGBA32);
	BMimeType type("application/x-vnd.Be-directory");
	type.GetIcon(folder, B_MINI_ICON);

	BButton* ChooseButton = new BButton("selectFiles", NULL, new BMessage(MSG_SELECT_FILES));
	ChooseButton->SetIcon(folder);

	BStringView* PathString = new BStringView("pathView", B_TRANSLATE("Choose files and folders"));
	PathString->SetFont(be_bold_font);
	PathString->SetExplicitMaxSize(BSize(550, 32));
	PathString->SetTruncation(B_TRUNCATE_MIDDLE);

	fFileView = new FileListView();
	fFileView->SetSelectionMessage(new BMessage(MSG_ROW_SELECTED));
	fFileView->SetInvocationMessage(new BMessage(MSG_OPEN));
	fFileView->SetSelectionMode(B_SINGLE_SELECTION_LIST);

	BScrollBar* scrollBar = (BScrollBar*)fFileView->FindView("horizontal_scroll_bar");

	// clang-format off
	BGroupLayout* compoundTitle = BLayoutBuilder::Group<>(B_HORIZONTAL,
			B_USE_SMALL_SPACING)
		.Add(ChooseButton)
		.AddGroup(B_VERTICAL)
			.AddGlue()
			.Add(PathString)
			.AddGlue()
			.End()
		.AddStrut(B_USE_SMALL_SPACING);
	// clang-format on

	static const float spacing = be_control_look->DefaultLabelSpacing();

	// clang-format off
	BGroupLayout* topBox = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS, spacing / 4,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
		.Add(fFileView);
	// clang-format on

	StatusView* statusView = new StatusView(scrollBar);
	fFileView->AddStatusView(statusView);

	BBox* top = new BBox("top");
	top->AddChild(topBox->View());
	top->SetLabel(compoundTitle->View());

	BMessage prefs;
	ReadPreferences("renamer_last", prefs);
	int32 activeRenamer;
	if (prefs.FindInt32("index", &activeRenamer) != B_OK)
		activeRenamer = 0;

	fRenamers = new BPopUpMenu("");
	BCardView* cards = new BCardView();
	fCards = cards->CardLayout();

	for (int i = 0; i < MODE_TOTAL; i++) {
		BMenuItem* item
			= new BMenuItem(((PecoApp*)be_app)->fRenamers[i]->fName, new BMessage(MSG_RENAMER));
		if (i == 0 || i == activeRenamer)
			item->SetMarked(true);
		fRenamers->AddItem(item);
		fCards->AddView(((PecoApp*)be_app)->fRenamers[i]);
	}

	((PecoApp*)be_app)->fRenameMode = activeRenamer;
	fCards->SetVisibleItem((int32)activeRenamer);

	BMenuField* menuField = new BMenuField("selectMode", "", fRenamers);
	BBox* bottom = new BBox("bottom");
	bottom->SetLabel(menuField);
	bottom->AddChild(cards);

	// StatusBar
	BStatusBar* statusBar = new BStatusBar("statusBar", "", NULL);
	statusBar->Hide();

	// Do it! - Button
	BButton* RenameButton = new BButton("DoIt", B_TRANSLATE("Rename"), new BMessage(MSG_DO_IT));
	RenameButton->SetEnabled(false);

	// clang-format off
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_SMALL_INSETS)
		.SetInsets(B_USE_WINDOW_INSETS, spacing / 4,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
		.Add(top, 100)
		.Add(bottom)
		.AddGroup(B_HORIZONTAL)
			.Add(statusBar, 100)
			.AddGlue()
			.Add(RenameButton, 0); // TODO how does weight work?
	// clang-format on
}


MainView::~MainView()
{
	int32 activeRenamer = fCards->VisibleIndex();
	BMessage msg;
	msg.AddInt32("index", activeRenamer);

	UpdatePreferences("renamer_last", msg);
}


void
MainView::AttachedToWindow()
{
	int32 num = fRenamers->CountItems();
	for (int32 i = 0; i < num; i++)
		fRenamers->ItemAt(i)->SetTarget(this);

	fFileView->SetTarget(this);
}


void
MainView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_RENAMER:
		{
			int32 selected = fRenamers->FindMarkedIndex();
			((PecoApp*)be_app)->fRenameMode = selected;
			fCards->SetVisibleItem(selected);
			MakeList();
			break;
		}
		case MSG_ROW_SELECTED:
		{
			BPoint where;
			uint32 buttons;
			fFileView->GetMouse(&where, &buttons);
			where.x += 2; // to prevent occasional select

			if (buttons & B_SECONDARY_MOUSE_BUTTON)
				_ShowPopUpMenu(fFileView->ConvertToScreen(where));
			break;
		}
		case MSG_REMOVE:
		{
			int32 index;
			if (message->FindInt32("index", &index) == B_OK) {
				((PecoApp*)be_app)->fWindow->Lock();

				FileListItem* Item = (FileListItem*)fFileView->ItemAt(index);
				if (Item == NULL)
					break;

				fFileView->RemoveItem(Item);
				((PecoApp*)be_app)->fList->RemoveItem(index);
				((PecoApp*)be_app)->fWindow->Unlock();
				MakeList();
			}
			break;
		}
		case MSG_OPEN: // relay the InvocationMessage of fFileView
		{
			int32 selection = fFileView->IndexOf(fFileView->CurrentSelection());
			message->AddInt32("index", selection);
			message->AddBool("location", false);
			BMessenger messenger((PecoApp*)be_app);
			messenger.SendMessage(message);
			break;
		}
		case MSG_POPCLOSED:
		{
			fShowingPopUpMenu = false;
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	}
}


#pragma mark-- Private Methods --


void
MainView::_ShowPopUpMenu(BPoint screen)
{
	if (fShowingPopUpMenu)
		return;

	int32 selection = fFileView->IndexOf(fFileView->CurrentSelection());

	ContextPopUp* menu = new ContextPopUp("PopUpMenu", this);
	BMessage* msg = NULL;

	msg = new BMessage(MSG_REMOVE);
	msg->AddInt32("index", selection);
	BMenuItem* item = new BMenuItem(B_TRANSLATE("Remove"), msg);
	item->SetTarget(this);
	menu->AddItem(item);

	msg = new BMessage(MSG_OPEN);
	msg->AddInt32("index", selection);
	msg->AddBool("location", false);
	item = new BMenuItem(B_TRANSLATE("Open"), msg, 'O');
	item->SetTarget(be_app);
	menu->AddItem(item);

	msg = new BMessage(MSG_OPEN);
	msg->AddInt32("index", selection);
	msg->AddBool("location", true);
	item = new BMenuItem(B_TRANSLATE("Open location"), msg, 'L');
	item->SetTarget(be_app);
	menu->AddItem(item);

	menu->Go(screen, true, true, true);
	fShowingPopUpMenu = true;
}
