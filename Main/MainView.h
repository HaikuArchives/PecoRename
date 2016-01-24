/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <View.h>

class BMessage;
class BCardLayout;
class BPopUpMenu;
class BTextView;

class MainView : public BView {
	public:
					MainView();
			void	MessageReceived( BMessage* message);
			void	AttachedToWindow();

	private:
		BCardLayout* fCards;
		BPopUpMenu* fRenamers;
};

#endif
