/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef UPPERLOWER_H
#define UPPERLOWER_H

#include <iostream.h>

#include <MenuField.h>

#include "Renamer.h"

class Renamer_UpperLower : public Renamer {
	public:
						Renamer_UpperLower();
			void		RenameList(BList *FileList);

			void		AttachedToWindow();
			void		DetachedFromWindow();


	private:
		BMenuField			*fUpperOrLower;
};

#endif
