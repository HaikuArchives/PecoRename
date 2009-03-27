/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <Box.h>
#include <Button.h>

#include "FileListItem.h"

enum {
	button_flat,
	button_out,
	button_in,
};

class FileView : public BView {
	public:
						FileView(BRect frame);
		virtual void	AttachedToWindow();
		virtual void	MessageReceived(BMessage *msg);
		virtual void	Draw(BRect frame);

		BPicture		*CreateButton(float width, const char* text, int mode);
		BPictureButton	*fSortButton;
};

#endif
