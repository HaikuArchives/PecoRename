/*
 */



#ifndef CONSISTENCYCHECK_H
#define CONSISTENCYCHECK_H



#include "FileListItem.h"

enum item_name_type{
		item_old_name,
		item_new_name,
		item_disk_name
};




class NameToCheck {
public:
		NameToCheck(item_name_type type, FileListItem *fileListItem):
			fType(type),
			fFileListItem(fileListItem)
		{
				
		};
const	BString& Name() {
			switch (fType) {
				case item_old_name: return fFileListItem->fName;
				case item_new_name:	return fFileListItem->fNewName;
				case item_disk_name: return fName;
			}
		}
		void SetDuplicate() {
			if (fType == item_new_name)
				fFileListItem->SetError(1);
		}

private: item_name_type fType;
		 BString fName;
		 FileListItem *fFileListItem;
	
};



class ConsistencyCheck {
	public:		
		virtual	~ConsistencyCheck();	
			void	AddNewName(FileListItem *fileListItem);
			void	AddOldName(FileListItem *fileListItem);
			void	AddDiskName(const BString name);
			bool    CheckForDuplicates();
			void    ResetNewName();
			void    Clear();
			void 	AddList(BList* nameList);
			void    AddNewList(BList* nameList);
			int 	CountOldNames() const {return fOldNames.CountItems();};
			void    PrintStatistic();
	private:
		BList fOldNames;
		BList fNewNames;
		BList fNames;
		int fNumDupFound;
};

#endif
