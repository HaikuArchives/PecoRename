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

//--------------------------------------------------------------------
//	Strings

// general
extern const char* STR_APP_NAME;
extern const char* STR_APP_VERSION;
extern const char* HELP_FILE;

// allgemein
extern const char* STR_YES;
extern const char* STR_NO;
extern const char* STR_OK;
extern const char* STR_WELL;
extern const char* STR_DATE_TIME_SECS_FORMAT;
extern const char* STR_DATE_TIME_FORMAT;
extern const char* STR_DATE_FORMAT;
extern const char* STR_CANCEL;
extern const char* STR_CONTINUE;
extern const char* STR_CONTINUE_WO_MSG;

// Panel
extern const char* STR_PANEL_TITLE;
extern const char* STR_SELECT;

// Menü
extern const char* STR_MENU_FILE;
extern const char* STR_MENU_NEW;
extern const char* STR_MENU_OPEN;
extern const char* STR_MENU_ABOUT;
extern const char* STR_MENU_QUIT;

extern const char* STR_MENU_TOOLS;
extern const char* STR_MENU_CREATE_SCRIPT;

extern const char* STR_MENU_HELP;
extern const char* STR_MENU_DOCU;

//StatusBar
extern const char* STATUS_STATUS;
extern const char* STATUS_SELECT_FILES;
extern const char* STATUS_IMPORT;
extern const char* STATUS_SELECT_MODE;
extern const char* STATUS_SORTING;
extern const char* STATUS_PREVIEW;
extern const char* STATUS_CHECKDUPS;
extern const char* STATUS_RENAMING;
extern const char* STATUS_WAITING;
extern const char* STATUS_DIDIT_FINE;
extern const char* STATUS_DIDIT_BAD;

// TopView
extern const char* STR_TOPVIEW_TITLE;
extern const char* STR_PLEASE_CHOOSE;
extern const char* STR_PATH;
extern const char* STR_NAME;
extern const char* STR_SIZE;
extern const char* STR_DATE;
extern const char* STR_PREVIEW;

// BottomView
extern const char* STR_BOTTOMVIEW_TITLE;
extern const char* STR_MODE;
extern const char* STR_PLEASE_SELECT;

// Weitere GUI-Elemente
extern const char* STR_DO_IT;
extern const char* STR_ABOUT_TITLE;
extern const char* STR_ABOUT_COPYRIGHT_0;
extern const char* STR_ABOUT_COPYRIGHT_1;
extern const char* STR_ABOUT_COPYRIGHT_2;
extern const char* STR_ABOUT_THANKYOU;

// Fehlermeldungen
extern const char* MESSAGE_MULTIDIR;
extern const char* MESSAGE_ERROR_IN_REGEX;
extern const char* MESSAGE_WILL_HAVE_PROBS;
extern const char* MESSAGE_HAVE_PROBLEM;
extern const char* MESSAGE_MARKED_FILES;
extern const char* MESSAGE_REALLY_DOIT;
extern const char* MESSAGE_NOTHING_TO_DO;
extern const char* MESSAGE_NOHELP;

// Renamer-Namen
extern const char* REN_SEARCH_REPLACE;
extern const char* REN_INSERTREPLACE;
extern const char* REN_NUMBERING;
extern const char* REN_EXTENSION;
extern const char* REN_UPPERLOWER;
extern const char* REN_REMOVE;

// Renamer-Settings
extern const char* REN_SET_REGEX;
extern const char* REN_SET_MATCHCASE;
extern const char* REN_SET_SEARCHPATTERN;
extern const char* REN_SET_REPLACESTRING;

extern const char* REN_SET_EXT_REPLACE_OLD;
extern const char* REN_SET_EXT_UPPERLOWER;
extern const char* REN_SET_DEFAULT;
extern const char* REN_SET_LOWERCASE;
extern const char* REN_SET_UPPERCASE;

extern const char* REN_SET_FORMAT;
extern const char* REN_SET_STARTWITH;
extern const char* REN_SET_TEXTBEFORE;
extern const char* REN_SET_TEXTBEHIND;

extern const char* REN_SET_INSERTREPLACE;
extern const char* REN_SET_INSERT;
extern const char* REN_SET_REPLACE;
extern const char* REN_SET_TEXT;
extern const char* REN_SET_ATPOSITION;
extern const char* REN_SET_FROMLEFT;
extern const char* REN_SET_FROMRIGHT;

extern const char* REN_SET_CONVERTTO;

extern const char* REN_SET_FROMPOS;
extern const char* REN_SET_TOPOS;

#endif
