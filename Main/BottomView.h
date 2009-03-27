/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef BOTTOM_VIEW_H
#define BOTTOM_VIEW_H

#include <Box.h>

#include "Renamer.h"

class BottomView : public BBox {
	public:
				BottomView( BRect frame );
				~BottomView();
				
		void 	AttachedToWindow();
};

#endif
