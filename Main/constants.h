/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _constants_h
#define _constants_h

#include <SupportDefs.h>

enum {
	MODE_SEARCH_REPLACE = 0,
	MODE_INSERTREPLACE,
	MODE_REMOVE,
	MODE_NUMBERING,
	MODE_EXTENSION,
	MODE_UPPERLOWER,
	MODE_TOTAL,
};

//--------------------------------------------------------------------
//	Message IDs

const uint32 MSG_MENU_NEW		= 'New ';
const uint32 MSG_MENU_SCRIPT	= 'CrSc';
const uint32 MSG_MENU_DOCU		= 'Docu';
const uint32 MSG_DO_IT			= 'DoIt';

const uint32 MSG_SELECT_FILES	= 'Sele';

const uint32 MSG_SORT_BY_NAME	= 'SNam';
const uint32 MSG_SORT_BY_SIZE	= 'SSiz';
const uint32 MSG_SORT_BY_DATE	= 'SDat';

const uint32 MSG_RENAMER			= 'Ren0';
const uint32 MSG_RENAME_SETTINGS	= 'Ren1';

//--------------------------------------------------------------------
//  Andere Konstanten

const float WIDTH_ICON			= 24.0;
const float WIDTH_NAME			= 176.0;
const float WIDTH_SIZE			= 70.0;
const float WIDTH_DATE			= 116.0;
const float WIDTH_PREVIEW		= 176.0;

#endif
