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
				ReportWindow(BRect frame, BList *filelist);
		virtual	~ReportWindow();

		bool	QuitRequested();
		void	MessageReceived( BMessage* msg );
		void 	RefsReceived(BMessage *msg);
private:
		void			Help();
	BColumnListView* 	fReportView;
};

#endif
