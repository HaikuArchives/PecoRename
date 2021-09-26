/*
 * Copyright 2017. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Janus
 */


#include <stdio.h>

#include "constants.h"
#include "functions.h"
#include "ConsistencyCheck.h"

int
SortByName(const void* item1, const void* item2)
{
	NameToCheck	**Item1 = (NameToCheck **)item1;
	NameToCheck	**Item2 = (NameToCheck **)item2;

	return (*Item1)->Name().Compare((*Item2)->Name());
}


ConsistencyCheck::ConsistencyCheck(BList* namesList)
{
	AddNamesList(namesList);
}


ConsistencyCheck::~ConsistencyCheck()
{
	for(int i = 0; i < fNames.CountItems(); i++)
		delete (NameToCheck*)fNames.ItemAt(i);

	fNames.MakeEmpty();
}


void
ConsistencyCheck::AddNewName(FileListItem* fileListItem)
{
	fNames.AddItem((void*)new NameToCheck(item_new_name, fileListItem));
}


void
ConsistencyCheck::AddOldName(FileListItem* fileListItem)
{
	fNames.AddItem((void*)new NameToCheck(item_old_name, fileListItem));
}


void ConsistencyCheck::AddNamesList(BList* nameList)
{
	for (int i = 0; i < nameList->CountItems(); i++)
	{
		FileListItem* listItem = (FileListItem*) nameList->ItemAt(i);
		listItem->SetError(0);
 		AddOldName(listItem);
 		if (listItem->fNewName != "")
 			AddNewName(listItem);
	}
}


/*
void
ConsistencyCheck::AddDiskName(const BString name)
{
}
*/


bool
ConsistencyCheck::CheckForDuplicates()
{
	fNames.SortItems(SortByName);
	if (fNames.CountItems() < 2)
		return false;

	bool duplicateFound = false;
	for (int i = 0; i < fNames.CountItems() - 1; i++)
	{
		NameToCheck* name1 = (NameToCheck*)fNames.ItemAt(i);
		NameToCheck* name2 = (NameToCheck*)fNames.ItemAt(i + 1);

		if (name1->Name().Compare(name2->Name()) == 0) {
			name1->SetDuplicate();
			name2->SetDuplicate();
			duplicateFound = true;
		}
	}
	return duplicateFound;
}
