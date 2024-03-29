/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2016, Markus Himmel
 *		2017 - 2023, Janus, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef _constants_h
#define _constants_h

#include <SupportDefs.h>

enum {
	MODE_SEARCH_REPLACE = 0,
	MODE_INSERTREPLACE,
	MODE_RANDOM,
	MODE_REMOVE,
	MODE_NUMBERING,
	MODE_EXTENSION,
	MODE_UPPERLOWER,
	MODE_TOTAL,
};

//--------------------------------------------------------------------
//	Message IDs

const uint32 MSG_MENU_NEW		= 'New ';
const uint32 MSG_MENU_DOCU		= 'Docu';
const uint32 MSG_DO_IT			= 'DoIt';

const uint32 MSG_SELECT_FILES	= 'Sele';

const uint32 MSG_SORT_BY_NAME	= 'SNam';
const uint32 MSG_SORT_BY_SIZE	= 'SSiz';
const uint32 MSG_SORT_BY_DATE	= 'SDat';

const uint32 MSG_RENAMER			= 'Ren0';
const uint32 MSG_RENAME_SETTINGS	= 'Ren1';
const uint32 MSG_CUSTOM_SYMBOLS		= 'Csym';

const uint32 MSG_OPEN				= 'Copn';
const uint32 MSG_REMOVE				= 'Crem';
const uint32 MSG_ROW_SELECTED		= 'Csel';
const uint32 MSG_POPCLOSED			= 'Cclo';


//--------------------------------------------------------------------
//  Other constants

const float WIDTH_ICON			= 16.0;
const float WIDTH_NAME			= 176.0;
const float WIDTH_SIZE			= 70.0;
const float WIDTH_DATE			= 84.0;
const float WIDTH_PREVIEW		= 176.0;


//--------------------------------------------------------------------
//  Chars used by the Random renamer

static const char en_upsymbols[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char en_lowsymbols[] = "abcdefghijklmnopqrstuvwxyz";
static const char num_symbols[] = "0123456789";
static const char spec_symbols[] = "!@#$%^&*"; //<-------

#endif // _constants_h
