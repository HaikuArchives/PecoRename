/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef TOP_VIEW_H
#define TOP_VIEW_H

#include <Box.h>
#include <Button.h>

class TopView : public BBox {
	public:
					TopView( BRect frame );
		void		AttachedToWindow();
		BPicture*	CreateButton(float width, const char* text, int mode);
};

#endif
