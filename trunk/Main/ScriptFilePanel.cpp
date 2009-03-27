/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Window.h>
#include <Button.h>

#include "constants.h"

#include "ScriptFilePanel.h"

ScriptFilePanel::ScriptFilePanel() : BFilePanel(B_SAVE_PANEL, NULL, NULL, B_FILE_NODE, false, 0, 0, true ) {
	SetButtonLabel(B_DEFAULT_BUTTON, STR_OK);

	BWindow		*myWindow = Window();

	myWindow->Lock();
	myWindow->SetTitle(STR_MENU_CREATE_SCRIPT);

	BButton		*CancelButton = (BButton *)myWindow->FindView("cancel button");
	CancelButton->SetLabel(STR_CANCEL);
	
	BRect		oldRect = CancelButton->Frame();
	CancelButton->ResizeToPreferred();
	CancelButton->MoveBy(oldRect.right-CancelButton->Frame().right, 0);
	myWindow->Unlock();
};
