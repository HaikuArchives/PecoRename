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
#include <CheckBox.h>
#include <LayoutBuilder.h>
#include <regex.h>
#include <string.h>
#include <TextControl.h>
#include <UTF8.h>

#include "constants.h"
#include "FileListItem.h"
#include "functions.h"

// TODO: this effectively turns off this replace mode, and should be fixed instead
#undef __INTEL__

// Für dieses Token wird das Suchergebnis als String ersetzt!
#define TOKEN "\\$"
// Dies ist das geschützte Token -> wird durch das Token ersetzt!
#define NO_TOKEN "\\\\$"

// Größe des Puffers für die regulären Ausdrücke
#define REG_BUFFER_SIZE 100

Renamer_SearchReplace::Renamer_SearchReplace()
:	Renamer() {

	fName 		= REN_SEARCH_REPLACE;

	MatchCase = new BCheckBox( NULL, REN_SET_MATCHCASE, new BMessage(MSG_RENAME_SETTINGS));

#if __INTEL__
	RegEx = new BCheckBox( REN_SET_REGEX, new BMessage(MSG_RENAME_SETTINGS));
#endif

	SearchFor = new BTextControl( REN_SET_SEARCHPATTERN, NULL, new BMessage(MSG_RENAME_SETTINGS));
	ReplaceWith = new BTextControl( NULL, REN_SET_REPLACESTRING, NULL, new BMessage(MSG_RENAME_SETTINGS));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
			.Add(MatchCase)
#if __INTEL__
			.Add(RegEx)
#endif
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(SearchFor)
			.Add(ReplaceWith);

}

void Renamer_SearchReplace::RenameList(BList *FileList) {

	Renamer :: RenameList(FileList);

	// Daten aus dem Formular einlesen
	const bool 	matchcase = bool(MatchCase->Value());		// Gross/Lleinschreibung beachten?
#if __INTEL__
	const bool	do_reg = bool(RegEx->Value()); 				// RegExpr oder nicht
#endif
	const char* utf_pattern = SearchFor->Text();			// Such-Pattern
	const char* utf_replacestring = ReplaceWith->Text();	// Ersetzstring

	FileListItem	*ListItem;

	if 	(strlen(utf_pattern)==0) return;

#if __INTEL__
 	if (do_reg) {

		// Pattern und Replace-String von UTF nach ASCII ISO1 umwandeln
		int32	laenge1, laenge2;

		laenge1 = laenge2 = strlen(utf_pattern);
		char	*pattern = new char[laenge2 + 1];
		convert_from_utf8(B_ISO1_CONVERSION, utf_pattern, &laenge1, pattern, &laenge2, 0);
		pattern[laenge2] = 0;

		laenge1 = laenge2 = strlen(utf_replacestring);
		char	*replacestring = new char[laenge2 + 1];
		convert_from_utf8(B_ISO1_CONVERSION, utf_replacestring, &laenge1, replacestring, &laenge2, 0);
		replacestring[laenge2] = 0;

		// Regulären Ausdruck anwenden
		unsigned char	buffer[REG_BUFFER_SIZE];

		regex_t			my_regex = { (unsigned char *)&buffer, REG_BUFFER_SIZE, 0, RE_SYNTAX_POSIX_AWK, NULL, NULL, 0, 1, REGS_FIXED, 0, 1, 0, 0, 0 };

		// Fehler im Regulären Ausdruck
		if 	(regcomp ( &my_regex, pattern, matchcase ? 0 : REG_ICASE ) != 0) return;

		regmatch_t		my_regmatch = { 0, 0 };

		for (int32 i = 0; i < fNumberOfItems; i++ ) {
			ListItem = (FileListItem *)FileList->ItemAt(i);

			BString		ResultString;

			laenge1 = laenge2 = strlen(ListItem->fName.String());
			char	*str = new char[laenge2 + 1];

			convert_from_utf8(B_ISO1_CONVERSION, ListItem->fName.String(), &laenge1, str, &laenge2, 0);
			str[laenge2] = 0;

			while ( regexec (&my_regex, str, 1, &my_regmatch, 0) == 0 ) {

				if  (my_regmatch.rm_eo == 0) { ResultString = ""; break; };
				int laenge = my_regmatch.rm_eo - my_regmatch.rm_so;
				char	*tokenstr = new char[laenge + 1];
				strncpy(tokenstr, str + my_regmatch.rm_so, laenge );
				tokenstr[laenge] = 0;

				BString		PatternString = replacestring;
				PatternString.ReplaceAll(NO_TOKEN, "\1");
				PatternString.ReplaceAll(TOKEN, tokenstr);
				PatternString.ReplaceAll("\1", TOKEN);

				str[ my_regmatch.rm_so ] = 0;

				ResultString << str << PatternString.String();

				str += my_regmatch.rm_eo;
			}

			ResultString << str;

			// Und alles wieder in UTF8 umwandeln

			laenge1 = strlen(ResultString.String());
			laenge2 = laenge1 * 2;
			char	*utf_String = new char[laenge2 + 1];

			convert_to_utf8(B_ISO1_CONVERSION, ResultString.String(), &laenge1, utf_String, &laenge2, 0);
			utf_String[laenge2] = 0;

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
