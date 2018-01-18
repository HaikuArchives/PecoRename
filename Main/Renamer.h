/*
 * Copyrights (c):
 *		2000 - 2008, Werner Freytag.
 *		2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original author:
 *		 Werner Freytag <freytag@gmx.de>
 */
#ifndef RENAMER_H
#define RENAMER_H

#include <List.h>
#include <Rect.h>
#include <String.h>
#include <View.h>

#include "constants.h"

class Renamer : public BView {
public:
						Renamer();
	virtual void		RenameList(BList* FileList);

			void		Draw(BRect updateRect);

	const char*			fName;
			int32		fNumberOfItems;
};

#endif // RENAMER_H
