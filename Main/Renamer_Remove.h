/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef REMOVE_H
#define REMOVE_H

#include <iostream.h>

#include <MenuField.h>

#include "Renamer.h"

class BTextControl;

class Renamer_Remove : public Renamer {
	public:
				Renamer_Remove();
		void	RenameList(BList *FileList);

		void	DetachedFromWindow();
		void	AttachedToWindow();

	private:
		BTextControl		*fPosition1, *fPosition2;
		BMenuField			*fDirection1, *fDirection2;
};

#endif
