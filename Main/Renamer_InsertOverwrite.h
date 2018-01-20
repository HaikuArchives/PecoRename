/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017, Janus
 *		2018, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef INSERTOVERWRITE_H
#define INSERTOVERWRITE_H

#include <MenuField.h>
#include <Spinner.h>

#include "Renamer.h"

class BTextControl;

class Renamer_InsertOverwrite : public Renamer {
public:
					Renamer_InsertOverwrite();
			void	RenameList(BList* FileList);

			void	AttachedToWindow();
			void	DetachedFromWindow();

private:
	BTextControl*	fText;
	BSpinner*		fPosition;
	BMenuField*		fInsertOrOverwrite;
	BMenuField*		fDirection;
};

#endif // INSERTOVERWRITE_H
