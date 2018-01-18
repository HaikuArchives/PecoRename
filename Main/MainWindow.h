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
#ifndef FENSTER_H
#define FENSTER_H

#include <ListView.h>
#include <Path.h>
#include <Window.h>

#include "MainView.h"
#include "FileListItem.h"

class MainWindow : public BWindow {
public:
				MainWindow(BRect frame);

		bool	QuitRequested();
		void	MessageReceived(BMessage* msg);
		void 	RefsReceived(BMessage* msg);

private:
		void	Help();
};

#endif // FENSTER_H
