/*
 * Distributed under the terms of the MIT License.
 */

#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <Window.h>
#include <Path.h>
#include <ListView.h>

#include "MainView.h"
#include "FileListItem.h"

class ReportWindow : public BWindow {
	public:
				ReportWindow(BList *filelist);
		bool	QuitRequested();
		void	MessageReceived( BMessage* msg );
		void 	RefsReceived(BMessage *msg);
	private:
		void	Help();
};

#endif
