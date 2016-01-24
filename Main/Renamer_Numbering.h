/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef NUMBERING_H
#define NUMBERING_H

#include <iostream.h>

#include <MenuField.h>

#include "Renamer.h"

class BTextControl;

class Renamer_Numbering : public Renamer {
	public:
					Renamer_Numbering();
		void		RenameList(BList *FileList);

		void		AttachedToWindow();
		void		DetachedFromWindow();

	private:
		BTextControl		*fTextBefore, *fTextBehind, *fStartWith;
		BMenuField			*fFormat;
};

#endif
