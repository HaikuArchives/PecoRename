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
#ifndef NUMBERING_H
#define NUMBERING_H

#include <MenuField.h>

#include "Renamer.h"

class BTextControl;

class Renamer_Numbering : public Renamer {
public:
				Renamer_Numbering();
	void		RenameList(BList* FileList);

	void		AttachedToWindow();
	void		DetachedFromWindow();

private:
	BTextControl*	fTextBefore;
	BTextControl*	fTextBehind;
	BTextControl*	fStartWith;
	BMenuField*		fFormat;
};

#endif // NUMBERING_H
