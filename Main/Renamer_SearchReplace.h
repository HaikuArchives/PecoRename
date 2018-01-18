/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef SEARCH_REPLACE_H
#define SEARCH_REPLACE_H

#include <CheckBox.h>

#include "Renamer.h"

class BTextControl;

class Renamer_SearchReplace : public Renamer {
public:
					Renamer_SearchReplace();
	void			RenameList(BList* FileList);

	void			AttachedToWindow();
	void			DetachedFromWindow();

private:
	BCheckBox*		MatchCase;
#if __INTEL__
	BCheckBox*		RegEx;
#endif
	BTextControl*	SearchFor;
	BTextControl*	ReplaceWith;
};

#endif // SEARCH_REPLACE_H
