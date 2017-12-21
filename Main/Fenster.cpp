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
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <Font.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Roster.h>
#include <TextControl.h>
#include <View.h>

#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "functions.h"

#include "Fenster.h"
#include "MainView.h"

Fenster::Fenster() : BWindow( BRect( 20, 40, 620, 460), "PecoRename", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS){
// Menü
	BMenuBar* MenuBar = new BMenuBar( "MenuBar" );

	BMenu* Menu;

	Menu = new BMenu(B_TRANSLATE("File"));
	MenuBar->AddItem(Menu);

	Menu->AddItem(new BMenuItem(B_TRANSLATE("New"), 				new BMessage(MSG_MENU_NEW), 'N'));
	Menu->AddItem(new BMenuItem(B_TRANSLATE("Select files..."), 				new BMessage(MSG_SELECT_FILES), 'O'));
	Menu->AddSeparatorItem();
	Menu->AddItem(new BMenuItem(B_TRANSLATE("About..."), 			new BMessage(B_ABOUT_REQUESTED)));
	Menu->AddSeparatorItem();
	Menu->AddItem(new BMenuItem(B_TRANSLATE("Quit"), 				new BMessage(B_QUIT_REQUESTED), 'Q'));

	Menu = new BMenu(B_TRANSLATE("Tools"));
	MenuBar->AddItem(Menu);

	Menu->AddItem(new BMenuItem(B_TRANSLATE("Create shell script..."), 	new BMessage(MSG_MENU_SCRIPT)));

//	Menu = new BMenu(B_TRANSLATE("Help"));
//	MenuBar->AddItem(Menu);

//	Menu->AddItem(new BMenuItem(B_TRANSLATE("Documentation"), new BMessage(MSG_MENU_DOCU)));

	MainView* mainView = new MainView();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(MenuBar)
		.Add(mainView);

	static_cast<BTextControl*>(mainView->FindView("pfadView"))->TextView()->MakeEditable(false);
};

void Fenster::Help() {
	app_info	myAppInfo;
	be_app->GetAppInfo(&myAppInfo);
	
	BPath	HelpFilePath;
	BPath(&myAppInfo.ref).GetParent(&HelpFilePath);
	HelpFilePath.Append(B_TRANSLATE("documentation/index.html"));
	
	entry_ref	ref;
	char		Signatur[B_MIME_TYPE_LENGTH];

	BMimeType("text/html").GetPreferredApp(Signatur);
	BMimeType(Signatur).GetAppHint(&ref);
	
	if ( (BPath(&ref).Path()==NULL) || (!BEntry(HelpFilePath.Path()).Exists()) ) {
		BAlert*	myAlert = new BAlert(NULL, B_TRANSLATE("An error has occurred:nEither the help file is missing, or an HTML browser can not be found."), B_TRANSLATE("Ok"));
		myAlert->Go(); return;
	}
	
	BString		Befehl(BPath(&ref).Path());
	Befehl.Append(" file://").Append(HelpFilePath.Path()).Append(" &");

	system(Befehl.String());
};


bool Fenster::QuitRequested() {
	return be_app->PostMessage(B_QUIT_REQUESTED);
};

void Fenster::MessageReceived ( BMessage* msg ) {
	switch (msg->what) {
	case MSG_MENU_DOCU:
		Help(); break;
	case B_COLORS_UPDATED:
		break;
	default:
		be_app->PostMessage(msg);
	}
}
