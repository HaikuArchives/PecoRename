/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _LIVE_TEXT_CONTROL_H
#define _LIVE_TEXT_CONTROL_H

#include <Control.h>
#include <TextView.h>

class LiveTextView;

class LiveTextControl : public BControl {

public:
						LiveTextControl(BRect frame,
									const char *name,
									const char *label, 
									const char *initial_text, 
									BMessage *message,
									uint32 rmask = B_FOLLOW_LEFT | B_FOLLOW_TOP,
									uint32 flags = B_WILL_DRAW | B_NAVIGABLE); 
virtual					~LiveTextControl();

virtual	void			SetText(const char *text);
		const char		*Text() const;

virtual	void			SetAlignment(alignment label, alignment text);

virtual	void			SetDivider(float dividing_line);

virtual	void			MouseDown(BPoint where);
virtual	void			Draw(BRect updateRect);
virtual	void			MakeFocus(bool focusState = true);

private:

LiveTextView			*fTextView;
};

class LiveTextView : public BTextView {

public:
				LiveTextView(BRect frame,
							 const char	*name,
							 BRect			textRect,
							 uint32		resizeMask,
							 uint32		flags = B_WILL_DRAW | B_PULSE_NEEDED);
							 
virtual	void			Draw(BRect updateRect);

virtual	void			MakeFocus(bool focusState = true);

virtual	void			InsertText(const char				*inText, 
								   int32					inLength, 
								   int32					inOffset,
								   const text_run_array		*inRuns);
virtual	void			DeleteText(int32 fromOffset, int32 toOffset);

};

#endif
