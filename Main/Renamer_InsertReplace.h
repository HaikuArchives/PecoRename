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
#ifndef INSERTREPLACE_H
#define INSERTREPLACE_H

#include <MenuField.h>

#include "Renamer.h"

class BTextControl;

class Renamer_InsertReplace : public Renamer {
public:
					Renamer_InsertReplace();
	void			RenameList(BList* FileList);

	void			AttachedToWindow();
	void			DetachedFromWindow();

private:
	BTextControl*	fText;
	BTextControl*	fPosition;
	BMenuField*		fInsertOrReplace;
	BMenuField*		fDirection;
};

#endif // INSERTREPLACE_H
