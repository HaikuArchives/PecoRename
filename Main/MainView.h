/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017, Janus
 *		2018, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <View.h>

#include "StatusView.h"

class BMessage;
class BCardLayout;
class BPopUpMenu;
class BTextView;

class MainView : public BView {
public:
					MainView();

			void	AttachedToWindow();
			void	MessageReceived(BMessage* message);

private:
			void	_ShowPopUpMenu(BPoint screen);

	BCardLayout* 	fCards;
	BPopUpMenu* 	fRenamers;
	FileListView* 	fFileView;
			bool	fShowingPopUpMenu;
};

#endif // MAIN_VIEW_H
