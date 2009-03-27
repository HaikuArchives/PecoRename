/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef FENSTER_H
#define FENSTER_H

#include <Window.h>
#include <Path.h>
#include <ListView.h>

#include "MainView.h"
#include "FileListItem.h"

class Fenster : public BWindow {
	public:
				Fenster();
		bool	QuitRequested();
		void	MessageReceived( BMessage* msg );
		void 	RefsReceived(BMessage *msg);
	private:
		void	Help();
};

#endif
