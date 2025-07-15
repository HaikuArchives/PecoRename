/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2018, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */


#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <Font.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <PathFinder.h>
#include <Roster.h>
#include <View.h>

#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "functions.h"

#include "MainView.h"
#include "MainWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(BRect frame)
	:
	BWindow(frame, B_TRANSLATE_SYSTEM_NAME("PecoRename"), B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS)
{

	BMenuBar* MenuBar = new BMenuBar("MenuBar");
	BMenu* Menu;

	Menu = new BMenu(B_TRANSLATE("File"));
	MenuBar->AddItem(Menu);
	//	Maybe in the future...Multi directories support
	//	Menu->AddItem(new BMenuItem(B_TRANSLATE("New"),
	//		new BMessage(MSG_MENU_NEW), 'N'));

	Menu->AddItem(new BMenuItem(B_TRANSLATE("Select files" B_UTF8_ELLIPSIS),
		new BMessage(MSG_SELECT_FILES), 'O'));
	Menu->AddSeparatorItem();

	Menu->AddItem(new BMenuItem(B_TRANSLATE("Show documentation"), new BMessage(MSG_MENU_DOCU)));

	Menu->AddItem(new BMenuItem(B_TRANSLATE("About PecoRename"), new BMessage(B_ABOUT_REQUESTED)));

	//	Menu = new BMenu(B_TRANSLATE("Help"));
	//	MenuBar->AddItem(Menu);

	Menu->AddSeparatorItem();
	Menu->AddItem(new BMenuItem(B_TRANSLATE("Quit"), new BMessage(B_QUIT_REQUESTED), 'Q'));

	MainView* mainView = new MainView();

	// clang-format off
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(MenuBar)
		.Add(mainView);
	// clang-format on
}


bool
MainWindow::QuitRequested()
{
	BMessage msg;
	msg.AddRect("pos", Frame());
	UpdatePreferences("main_window", msg);

	return be_app->PostMessage(B_QUIT_REQUESTED);
}


void
MainWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case MSG_MENU_DOCU:
			_ShowDocumentation();
			break;
		case B_COLORS_UPDATED:
			break;

		default:
			be_app->PostMessage(msg);
	}
}


#pragma mark-- Private Methods --


void
MainWindow::_ShowDocumentation()
{
	BPathFinder pathFinder;
	BStringList paths;
	BPath path;
	BEntry entry;

	status_t error
		= pathFinder.FindPaths(B_FIND_PATH_DOCUMENTATION_DIRECTORY, "packages/pecorename", paths);

	for (int i = 0; i < paths.CountStrings(); ++i) {
		if (error == B_OK && path.SetTo(paths.StringAt(i)) == B_OK
			&& path.Append(B_TRANSLATE_COMMENT("ReadMe.html",
				   "Path to the help file. Only change if a translated file is "
				   "provided."))
				== B_OK) {
			entry = path.Path();
			entry_ref ref;
			entry.GetRef(&ref);
			be_roster->Launch(&ref);
		}
	}
}
