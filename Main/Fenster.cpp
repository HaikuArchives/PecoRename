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
#include "LiveTextControl.h"
#include "MainView.h"

Fenster::Fenster() : BWindow( BRect( 20, 40, 620, 460), "PecoRename", B_TITLED_WINDOW, 0){
// Menü
	BMenuBar* MenuBar = new BMenuBar( BRect( 0, 0, 0, 0 ), "MenuBar" );

	BMenu* Menu;

	Menu = new BMenu(STR_MENU_FILE);
	MenuBar->AddItem(Menu);

	Menu->AddItem(new BMenuItem(STR_MENU_NEW, 				new BMessage(MSG_MENU_NEW), 'N'));
	Menu->AddItem(new BMenuItem(STR_MENU_OPEN, 				new BMessage(MSG_SELECT_FILES), 'O'));
	Menu->AddSeparatorItem();
	Menu->AddItem(new BMenuItem(STR_MENU_ABOUT, 			new BMessage(B_ABOUT_REQUESTED)));
	Menu->AddSeparatorItem();
	Menu->AddItem(new BMenuItem(STR_MENU_QUIT, 				new BMessage(B_QUIT_REQUESTED), 'Q'));

	Menu = new BMenu(STR_MENU_TOOLS);
	MenuBar->AddItem(Menu);

	Menu->AddItem(new BMenuItem(STR_MENU_CREATE_SCRIPT, 	new BMessage(MSG_MENU_SCRIPT)));

	Menu = new BMenu(STR_MENU_HELP);
	MenuBar->AddItem(Menu);

	Menu->AddItem(new BMenuItem(STR_MENU_DOCU, new BMessage(MSG_MENU_DOCU)));

	AddChild( MenuBar );
// Größe bestimmen + Fenster anpassen
	float BarWidth, BarHeight;
	MenuBar->GetPreferredSize ( &BarWidth, &BarHeight );
	SetSizeLimits( 600, 600, 10000, 155 + BarHeight + 5 * be_plain_font->Size() + 2 * be_bold_font->Size());

// Main Frame
	BRect MainFrame = Bounds();
	MainFrame.top += BarHeight;

	MainView* mainView = new MainView( MainFrame );
	AddChild( mainView );
	
	BTextControl*	TextControl = (BTextControl *)FindView("pfadView");
	TextControl->TextView()->MakeEditable(false);

};

void Fenster::Help() {
	app_info	myAppInfo;
	be_app->GetAppInfo(&myAppInfo);
	
	BPath	HelpFilePath;
	BPath(&myAppInfo.ref).GetParent(&HelpFilePath);
	HelpFilePath.Append(HELP_FILE);
	
	entry_ref	ref;
	char		Signatur[B_MIME_TYPE_LENGTH];

	BMimeType("text/html").GetPreferredApp(Signatur);
	BMimeType(Signatur).GetAppHint(&ref);
	
	if ( (BPath(&ref).Path()==NULL) || (!BEntry(HelpFilePath.Path()).Exists()) ) {
		BAlert*	myAlert = new BAlert(NULL, MESSAGE_NOHELP, STR_OK);
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
	default:
		be_app->PostMessage(msg);
	}
}
