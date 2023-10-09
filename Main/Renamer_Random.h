/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 *		2016, Markus Himmel
 *		2017, Janus
 *		2018-2023, Humdinger
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 * 		Werner Freytag <freytag@gmx.de>
 */
#ifndef RANDOM_H
#define RANDOM_H

#include <CheckBox.h>
#include <MenuField.h>
#include <Spinner.h>
#include <TextControl.h>

#include "Renamer.h"

class Renamer_Random : public Renamer {
public:
					Renamer_Random();
			void	RenameList(BList* FileList);

	virtual void 	MessageReceived(BMessage* msg);
			void	AttachedToWindow();
			void	DetachedFromWindow();

private:
	BSpinner*		fPosition;
	BSpinner*		fCount;

	BCheckBox* 		fUpperCase;
	BCheckBox* 		fLowerCase;
	BCheckBox* 		fNumbers;
	BCheckBox* 		fSpecialSymbols;
	BCheckBox* 		fCustom;

	BTextControl* 	fCustomInput;

	BMenuField*		fInsertOrOverwrite;
	BMenuField*		fDirection;
};

#endif // RANDOM_H
