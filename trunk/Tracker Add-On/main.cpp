/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Roster.h>
#include <Entry.h>
#include <TrackerAddOn.h>
#include <Alert.h>
#include <Messenger.h>

#define APP_SIGNATURE "application/x-vnd.pecora-PecoRename"

void process_refs(entry_ref dir_ref, BMessage *msg, void *) {

	int			refs;
	entry_ref	file_ref;
	BMessage	theMessage(B_SIMPLE_DATA);
	team_id		team;
	
	for ( refs = 0; msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR; refs++ )
		theMessage.AddRef("refs", &file_ref);

	new BRoster();
	if (be_roster->Launch(APP_SIGNATURE, (BMessage *)NULL, &team) == B_OK)
		(new BMessenger(APP_SIGNATURE, team))->SendMessage(&theMessage);
	else
		(new BAlert("", "I'm sorry, I can't find PecoRename.\n\nIs it already installed on this system?\n\nYou can download it from\nhttp://www.pecora.de/pecorename !", "Ok, I will do that", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
};
