/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <ListView.h>

class FileListView : public BListView {
public:
					FileListView(BRect frame);
	virtual void	MouseDown(BPoint where);
	virtual	void	KeyDown(const char *bytes, int32 numBytes);
	virtual bool	InitiateDrag(BPoint pt, int32 itemIndex, bool initialySelected);
};
