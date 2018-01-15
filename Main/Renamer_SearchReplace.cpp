/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "Renamer_SearchReplace.h"

#include <Alert.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <LayoutBuilder.h>
#include <regex.h>
#include <string.h>
#include <TextControl.h>
#include <UTF8.h>

#include "constants.h"
#include "FileListItem.h"
#include "functions.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Renamer_SearchReplace"

// TODO: this effectively turns off this replace mode, and should be fixed instead
#undef __INTEL__

// For this token the search result will be replaced as string!
#define TOKEN "\\$"
// This is the protected token -> will be replaced with the token!
#define NO_TOKEN "\\\\$"

// Buffer size for the regular expressions
#define REG_BUFFER_SIZE 100

Renamer_SearchReplace::Renamer_SearchReplace()
:	Renamer() {

	fName 		= B_TRANSLATE("Search and replace");

	MatchCase = new BCheckBox( NULL, B_TRANSLATE("Case sensitive"), new BMessage(MSG_RENAME_SETTINGS));

#if __INTEL__
	RegEx = new BCheckBox( B_TRANSLATE("Regular expression"), new BMessage(MSG_RENAME_SETTINGS));
#endif

	SearchFor = new BTextControl( B_TRANSLATE("Find pattern:"), NULL, new BMessage(MSG_RENAME_SETTINGS));
	SearchFor->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));
	ReplaceWith = new BTextControl( NULL, B_TRANSLATE("Replace with:"), NULL, new BMessage(MSG_RENAME_SETTINGS));
	ReplaceWith->SetModificationMessage(new BMessage(MSG_RENAME_SETTINGS));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(SearchFor)
			.Add(ReplaceWith)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(MatchCase)
#if __INTEL__
			.Add(RegEx)
#endif
			.AddGlue()
		.End()
		.AddGlue();
}

void Renamer_SearchReplace::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	// Read data from the form
	const bool 	matchcase = bool(MatchCase->Value());
#if __INTEL__
	const bool	do_reg = bool(RegEx->Value());
#endif
	const char* utf_pattern = SearchFor->Text();
	const char* utf_replacestring = ReplaceWith->Text();

	FileListItem	*ListItem;

	if 	(strlen(utf_pattern)==0) return;

#if __INTEL__
 	if (do_reg) {

		// Convert pattern and replace string from UTF to ASCII ISO1
		int32	length1, length2;

		length1 = length2 = strlen(utf_pattern);
		char	*pattern = new char[length2 + 1];
		convert_from_utf8(B_ISO1_CONVERSION, utf_pattern, &length1, pattern, &length2, 0);
		pattern[length2] = 0;

		length1 = length2 = strlen(utf_replacestring);
		char	*replacestring = new char[length2 + 1];
		convert_from_utf8(B_ISO1_CONVERSION, utf_replacestring, &length1, replacestring, &length2, 0);
		replacestring[length2] = 0;

		// Apply the regular expression
		unsigned char	buffer[REG_BUFFER_SIZE];

		regex_t			my_regex = { (unsigned char *)&buffer, REG_BUFFER_SIZE, 0, RE_SYNTAX_POSIX_AWK, NULL, NULL, 0, 1, REGS_FIXED, 0, 1, 0, 0, 0 };

		// Error in the regular expression
		if 	(regcomp ( &my_regex, pattern, matchcase ? 0 : REG_ICASE ) != 0) return;

		regmatch_t		my_regmatch = { 0, 0 };

		for (int32 i = 0; i < fNumberOfItems; i++ ) {
			ListItem = (FileListItem *)FileList->ItemAt(i);

			BString		ResultString;

			length1 = length2 = strlen(ListItem->fName.String());
			char	*str = new char[length2 + 1];

			convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(), &length1, str, &length2, 0);
			str[length2] = 0;

			while ( regexec (&my_regex, str, 1, &my_regmatch, 0) == 0 ) {

				if  (my_regmatch.rm_eo == 0) { ResultString = ""; break; };
				int length = my_regmatch.rm_eo - my_regmatch.rm_so;
				char	*tokenstr = new char[length + 1];
				strncpy(tokenstr, str + my_regmatch.rm_so, length );
				tokenstr[length] = 0;

				BString		PatternString = replacestring;
				PatternString.ReplaceAll(NO_TOKEN, "\1");
				PatternString.ReplaceAll(TOKEN, tokenstr);
				PatternString.ReplaceAll("\1", TOKEN);

				str[ my_regmatch.rm_so ] = 0;

				ResultString << str << PatternString.String();

				str += my_regmatch.rm_eo;
			}

			ResultString << str;

			// And convert everything back to UTF8

			length1 = strlen(ResultString.String());
			length2 = length1 * 2;
			char	*utf_String = new char[length2 + 1];

			convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(), &length1, utf_String, &length2, 0);
			utf_String[length2] = 0;

			ListItem->SetNewName( utf_String );
		}

	} else {
#endif
		for (int32 i = 0; i < fNumberOfItems; i++ ) {

			ListItem = (FileListItem *)FileList->ItemAt(i);

			BString		ResultString( ListItem->fName );

			if (matchcase)
				ResultString.ReplaceAll(utf_pattern, utf_replacestring);
			else
				ResultString.IReplaceAll(utf_pattern, utf_replacestring);

			ListItem->SetNewName( ResultString );

		}
#if __INTEL__
	}
#endif
};

void Renamer_SearchReplace::DetachedFromWindow() {
	BMessage	msg;
	msg.AddBool("matchcase", bool(MatchCase->Value()));
#if __INTEL__
	msg.AddBool("do_reg", bool(RegEx->Value()));
#endif
	msg.AddString("pattern", SearchFor->Text());
	msg.AddString("replacestring", ReplaceWith->Text());
	UpdatePreferences("ren_searchreplace", msg);
}

void Renamer_SearchReplace::AttachedToWindow() {
	BMessage	msg;
	ReadPreferences("ren_searchreplace", msg);

	BString string;
	bool boolean;
	if (msg.FindBool("matchcase", &boolean)==B_OK)
		MatchCase->SetValue(boolean);
#if __INTEL__
	if (msg.FindBool("do_reg", &boolean)==B_OK)
		RegEx->SetValue(boolean);
#endif
	if (msg.FindString("pattern", &string)==B_OK)
		SearchFor->SetText(string.String());
	if (msg.FindString("replacestring", &string)==B_OK)
		ReplaceWith->SetText(string.String());
}
