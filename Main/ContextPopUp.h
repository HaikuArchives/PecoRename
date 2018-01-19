/*
 * Copyright 2011. Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *	Philippe Houdoin
 */

#ifndef CONTEXTPOPUP_H
#define CONTEXTPOPUP_H

#include <Messenger.h>
#include <PopUpMenu.h>


class ContextPopUp : public BPopUpMenu {
public:
					ContextPopUp(const char* name, BMessenger target);
	virtual 		~ContextPopUp();

private:
	BMessenger 		fTarget;
};


#endif // CONTEXTPOPUP_H
