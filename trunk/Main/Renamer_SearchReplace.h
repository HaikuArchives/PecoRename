/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef SEARCH_REPLACE_H
#define SEARCH_REPLACE_H

#include <CheckBox.h>

#include "Renamer.h"

class LiveTextControl;

class Renamer_SearchReplace : public Renamer {
	public:
					Renamer_SearchReplace();
		void		RenameList(BList *FileList);

		void		AttachedToWindow();
		void		DetachedFromWindow();

	private:
		BCheckBox			*MatchCase;
#if __INTEL__
		BCheckBox			*RegEx;
#endif
		LiveTextControl		*SearchFor, *ReplaceWith;		
};

#endif
