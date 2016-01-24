/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef PECO_APP_H
#define PECO_APP_H

#include <Application.h>
#include <Path.h>

#include "constants.h"
#include "FileListView.h"

class BFilePanel;
class BStatusBar;
class Fenster;
class Renamer;

class PecoApp : public BApplication {
	public:
						PecoApp();
		virtual bool	QuitRequested();
		virtual void	MessageReceived(BMessage *msg);
		virtual void	RefsReceived(BMessage *msg);
		virtual void	ReadyToRun();
		virtual	void	AboutRequested();

		Fenster			*fWindow;
		FileListView	*fListView;
		BList			*fList;
		BFilePanel		*fFilePanel, *fScriptFilePanel;
		BPath			fPfad;
		int32			fRenameMode;
		BStatusBar		*fStatusBar;
		Renamer			*fRenamers[MODE_TOTAL];

	private:
		void			New();
		bool			NothingToDo();
		void			CreateScript(BMessage *msg);
		void			DoIt();
};

#endif
