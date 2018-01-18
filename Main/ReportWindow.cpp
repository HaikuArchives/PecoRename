/*
 * Copyright 2017. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Janus
 */


#include <Button.h>
#include <Catalog.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>
#include <LayoutBuilder.h>
#include <MessageFormat.h>
#include <StringView.h>

#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "functions.h"
#include "PreviewColumn.h"
#include "ReportWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ReportWindow"


ReportWindow::ReportWindow(BRect frame, BList* filelist)
	:
	BWindow(frame, B_TRANSLATE("Report"), B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS)
{
	fReportView = new BColumnListView("reportView",
		B_FRAME_EVENTS | B_NAVIGABLE, B_NO_BORDER);

	// info columns
	int32 i = 0;
	fReportView->AddColumn(new BStringColumn(B_TRANSLATE_COMMENT(
		"Original name", "Column title"), 165, 10, 600, B_TRUNCATE_END), i++);
	fReportView->AddColumn(new PreviewColumn(B_TRANSLATE_COMMENT(
		"Targetted name", "Column title"), 165, 10, 600, B_TRUNCATE_END), i++);
	fReportView->AddColumn(new BStringColumn(B_TRANSLATE_COMMENT(
		"Problem", "Column title"), 250, 10, 600, B_TRUNCATE_END), i++);

	BMessage msg;
	ReadPreferences("report_window", msg);

	BMessage colSettings;
	if (msg.FindMessage("col", &colSettings) == B_OK)
		fReportView->LoadState(&colSettings);
	
	FileListItem* listItem;
	int32 errorCount = 0;
	for (int32 i = 0; (listItem = (FileListItem*)filelist->ItemAt(i))
			!= NULL; i++) {
		if (listItem->Error() != 0 && listItem->fNewName != "") {
				BRow* row = new BRow();
				PreviewField* previewField
					= new PreviewField(listItem->fNewName);
				previewField->SetError(true);

				int32 i = 0;
				row->SetField(new BStringField(listItem->fName), i++);
				row->SetField(previewField, i++);
				BString errorMsg;
				switch (listItem->Error()) {
					case 0 : // this should never happen
						break; 
					case 1 :
					{	// Problem was already spotted for the preview in the
						// main window. Happens when the existing name was part
						// of the list of files
						errorMsg = B_TRANSLATE(
							"File already exists (as in preview)");
						errorCount++;
						break;
					}
					case 2 :
					{	// Problem only cropped up after clicking "Rename"
						// Happens when the existing name was NOT part of the
						// list of files
						errorMsg = B_TRANSLATE("File already exists");
						errorCount++;
						break;
					}
					default : errorMsg = B_TRANSLATE("Unknown error");
				}
				row->SetField(new BStringField(errorMsg), i++);
				fReportView->AddRow(row);
			}
		}

	BString errorString;
	static BMessageFormat errorMessage(B_TRANSLATE("{0, plural,"
		"=1{A problem was encountered during processing:  "
			"One file couldn't be renamed}"
		"other{Problems were encountered during processing:  "
			"# files couldn't be renamed}}"));
	errorMessage.Format(errorString, errorCount);

	BStringView* messageView = new BStringView("Errors", errorString);
	messageView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(messageView)
		.Add(fReportView)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(new BButton("OK", B_TRANSLATE("OK"), new BMessage('CLO_')))
		.End(); 
}


ReportWindow::~ReportWindow()
{
	BMessage msg;
	BMessage colSettings;
	fReportView->SaveState(&colSettings);

	msg.AddMessage("col", &colSettings);
	msg.AddRect("size", Bounds());
	UpdatePreferences("report_window", msg);
}


bool
ReportWindow::QuitRequested()
{
	return true;
}


void
ReportWindow::MessageReceived (BMessage* msg)
{
	switch (msg->what) {
		case 'CLO_':
			Close();
			break;
		case B_COLORS_UPDATED:
			break;
		default:
			BWindow::PostMessage(msg);
	}
}
