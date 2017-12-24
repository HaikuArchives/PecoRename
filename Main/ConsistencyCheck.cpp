/*
 */
#include <stdio.h>

#include "constants.h"
#include "functions.h"
#include "ConsistencyCheck.h"

int SortByName(const void *item1, const void *item2) {

	NameToCheck	**Item1 = (NameToCheck **)item1;
	NameToCheck	**Item2 = (NameToCheck **)item2;

	return (*Item1)->Name().Compare( (*Item2)->Name() );
}

ConsistencyCheck::~ConsistencyCheck()
{
	for(int i = 0; i < fNewNames.CountItems(); i++)
		delete (NameToCheck*)fNewNames.ItemAt(i);
	fNewNames.MakeEmpty();
	for(int i = 0; i < fOldNames.CountItems(); i++)
		delete (NameToCheck*)fOldNames.ItemAt(i);
	fOldNames.MakeEmpty();
	fNames.MakeEmpty();
}




void
ConsistencyCheck::AddNewName(FileListItem *fileListItem)
{
	fNewNames.AddItem((void *)new NameToCheck(item_new_name, fileListItem)); 
}

void
ConsistencyCheck::AddOldName(FileListItem *fileListItem)
{
	fOldNames.AddItem((void *)new NameToCheck(item_old_name, fileListItem));
}


void
ConsistencyCheck::ResetNewName()
{
	for(int i = 0; i < fNewNames.CountItems(); i++)
		delete (NameToCheck*)fNewNames.ItemAt(i);
	fNewNames.MakeEmpty();
	
	fNames.MakeEmpty();
	//fNames.AddList(&fOldNames);
	fNames=fOldNames;
}


void ConsistencyCheck::AddList(BList* nameList)
{
	for (int i = 0; i < nameList->CountItems(); i++)
	{
		FileListItem* listItem = (FileListItem*) nameList->ItemAt(i);
		listItem->SetError(0);
 		AddOldName(listItem);
	}
}
/*
void ConsistencyCheck::SetOldNamesList(BList* nameList)
{
	for(int i = 0; i < fOldNames.CountItems(); i++)
		delete (NameToCheck*)fOldNames.ItemAt(i);
	fOldNames.MakeEmpty();

	for (int i = 0; i < nameList->CountItems(); i++)
	{
		FileListItem* listItem = (FileListItem*) nameList->ItemAt(i);
		listItem->SetError(0);
 		AddOldName(listItem);
	}
}
*/


void ConsistencyCheck::AddNewList(BList* nameList)
{
	for (int i = 0; i < nameList->CountItems(); i++)
	{
		FileListItem* listItem = (FileListItem*) nameList->ItemAt(i);
 		if (listItem->fNewName != "") {
 			listItem->SetError(0); 
 			AddNewName(listItem);
 		}
	}
}

void
ConsistencyCheck::AddDiskName(const BString name)
{
}


void
ConsistencyCheck::PrintStatistic()
{
	printf("fOldNames %d\n", fOldNames.CountItems());
	printf("fNewNames %d\n", fNewNames.CountItems());
	printf("fNames %d\n", fNames.CountItems());
	printf("fNumDupFound %d\n", fNumDupFound);
}


bool
ConsistencyCheck::CheckForDuplicates()
{
	fNames.AddList(&fNewNames);
	fNames.SortItems(SortByName);
	if (fNames.CountItems() < 2)
		return false;
 	fNumDupFound = 0;
 	bool alreadyFound = false;
	for (int i = 0; i < fNames.CountItems() - 1; i++) {
		NameToCheck* name1 = (NameToCheck*)fNames.ItemAt(i);
		NameToCheck* name2 = (NameToCheck*)fNames.ItemAt(i+1);

		if ( name1->Name().Compare(name2->Name()) == 0)		
		{
			name1->SetDuplicate();
			name2->SetDuplicate();
			if (alreadyFound == true) {
				fNumDupFound++;
			} else {
				alreadyFound = true;
				fNumDupFound +=2;
			}			
		}
		else alreadyFound = false;
	}
}
