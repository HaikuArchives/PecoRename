/*
 */

#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>
#include <Font.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Roster.h>
#include <StringView.h>
#include <TextControl.h>
#include <View.h>

#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "functions.h"
#include "PreviewColumn.h"
#include "ReportWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ReportWindow"


ReportWindow::ReportWindow(BList *filelist) : BWindow( BRect( 60, 80, 600, 280), B_TRANSLATE("Report"),
	 B_TITLED_WINDOW,B_AUTO_UPDATE_SIZE_LIMITS)
{

	BStringView *messageView = new BStringView("Errors", B_TRANSLATE("There were problems during the renaming"));
	BBox* reportBox = new BBox("Errors");
	reportBox->SetLabel(B_TRANSLATE("There were problems during the renaming"));
	messageView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
	BColumnListView* reportView = new BColumnListView("reportView", B_FRAME_EVENTS|B_NAVIGABLE, B_NO_BORDER);

	//info columns
	int32 i = 0;
	reportView->AddColumn(new BStringColumn(B_TRANSLATE("Name"), 165, 10, 600, B_TRUNCATE_END), i++);
	reportView->AddColumn(new PreviewColumn(B_TRANSLATE("Failed rename to"), 165, 10, 600, B_TRUNCATE_END), i++);
	reportView->AddColumn(new BStringColumn(B_TRANSLATE("Problem"), 185, 10, 600, B_TRUNCATE_END), i++);
	
	FileListItem	*listItem;
	for (int32 i = 0; (listItem = (FileListItem *)filelist->ItemAt(i)) != NULL; i++ ) {
		if (listItem->Error() != 0 && listItem->fNewName != "")
			{
				BRow* row = new BRow();
				PreviewField* previewField = new PreviewField(listItem->fNewName);
				previewField->SetError(true);
				int32 i = 0;
				row->SetField(new BStringField(listItem->fName), i++);
				row->SetField(previewField, i++);
				BString errorMsg;
				switch (listItem->Error()) {
					case 0 : // this should never happen
						break; 
					case 1 : errorMsg = B_TRANSLATE("Duplicated name");
						break;
					case 2 : errorMsg = B_TRANSLATE("A file with that name already exists");
						break;
					default : errorMsg = B_TRANSLATE("Unknown error");
				}
				row->SetField(new BStringField(errorMsg), i++);
				reportView->AddRow(row);
			}
	
	}
	
	BGroupLayout *topBox = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(reportView);
	reportBox->AddChild(topBox->View());

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
//		.Add(messageView)
//		.Add(reportView)
		.Add(reportBox)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add( new BButton( "OK", B_TRANSLATE("OK"), new BMessage('CLO_'))); 
};




bool ReportWindow::QuitRequested() {
	return true;
};

void ReportWindow::MessageReceived ( BMessage* msg ) {
	switch (msg->what) {
	case 'CLO_':
		Close(); break;
	case B_COLORS_UPDATED:
		break;
	default:
		BWindow::PostMessage(msg);
	}
}
