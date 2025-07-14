/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Alert.h>
#include <Catalog.h>
#include <Entry.h>
#include <Messenger.h>
#include <Roster.h>
#include <TrackerAddOn.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Tracker_addon"

#define APP_SIGNATURE "application/x-vnd.pecora-PecoRename"


void
process_refs(entry_ref dir_ref, BMessage* msg, void*)
{

	int refs;
	entry_ref file_ref;
	BMessage theMessage(B_SIMPLE_DATA);
	team_id team;

	for (refs = 0; msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR; refs++)
		theMessage.AddRef("refs", &file_ref);

	new BRoster();
	if (be_roster->Launch(APP_SIGNATURE, (BMessage*)NULL, &team) == B_OK) {
		(new BMessenger(APP_SIGNATURE, team))->SendMessage(&theMessage);
	} else {
		(new BAlert("",
			 B_TRANSLATE("Can't find PecoRename.\n\n"
						 "It looks like it's not properly installed.\n"
						 "The PecoRename package can be downloaded and installed "
						 "with HaikuDepot."),
			 B_TRANSLATE("OK"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))
			->Go();
	}
}
