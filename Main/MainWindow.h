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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ListView.h>
#include <Path.h>
#include <Window.h>

#include "FileListItem.h"
#include "MainView.h"

class MainWindow : public BWindow {
public:
			MainWindow(BRect frame);

	bool	QuitRequested();
	void	MessageReceived(BMessage* msg);

private:
	void	_ShowDocumentation();
};

#endif // MAINWINDOW_H
