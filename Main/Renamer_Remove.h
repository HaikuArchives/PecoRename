/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017, Janus
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef REMOVE_H
#define REMOVE_H

#include <MenuField.h>
#include <Spinner.h>

#include "Renamer.h"

class BTextControl;

class Renamer_Remove : public Renamer {
public:
					Renamer_Remove();
			void	RenameList(BList* FileList);

			void	AttachedToWindow();
			void	DetachedFromWindow();

private:
		BSpinner*	fPosition1;
		BSpinner*	fPosition2;
	BMenuField*		fDirection1;
	BMenuField*		fDirection2;
};

#endif // REMOVE_H
