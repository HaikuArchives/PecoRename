/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2011, Axel Dörfler
 *		2016, Markus Himmel, Hannah
 *		2017 - 2018, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef PECO_APP_H
#define PECO_APP_H

#include <Application.h>
#include <Path.h>

#include "constants.h"

#include "FileListView.h"

class BFilePanel;
class BStatusBar;
class MainWindow;
class Renamer;

class PecoApp : public BApplication {
public:
					PecoApp();

	virtual bool	QuitRequested();
	virtual void	MessageReceived(BMessage* msg);
	virtual void	RefsReceived(BMessage* msg);
	virtual void	ReadyToRun();
	virtual	void	AboutRequested();

			bool	NothingToDo();
			void	New();
			void	DoIt();

	MainWindow*		fWindow;
	FileListView*	fListView;
	BList*			fList;
	BFilePanel*		fFilePanel;
	BPath			fPath;
	int32			fRenameMode;
	BStatusBar*		fStatusBar;
	Renamer*		fRenamers[MODE_TOTAL];
};

#endif // PECO_APP_H
