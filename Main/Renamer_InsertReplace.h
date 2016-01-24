/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef INSERTREPLACE_H
#define INSERTREPLACE_H

#include <iostream.h>

#include <MenuField.h>

#include "Renamer.h"

class BTextControl;

class Renamer_InsertReplace : public Renamer {
	public:
					Renamer_InsertReplace();
		void		RenameList(BList *FileList);

		void		AttachedToWindow();
		void		DetachedFromWindow();

	private:
		BTextControl		*fText, *fPosition;
		BMenuField			*fInsertOrReplace, *fDirection;
};

#endif
