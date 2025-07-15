/*
 * Copyright 2011. Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *	Philippe Houdoin
 */

#include "ContextPopUp.h"
#include "constants.h"


ContextPopUp::ContextPopUp(const char* name, BMessenger target)
	:
	BPopUpMenu(name, false, false),
	fTarget(target)
{
	SetAsyncAutoDestruct(true);
}


ContextPopUp::~ContextPopUp()
{
	fTarget.SendMessage(MSG_POPCLOSED);
}
