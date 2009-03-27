/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "LiveTextControl.h"

#include <string.h>

LiveTextControl::LiveTextControl(BRect frame,
							const char *name,
							const char *label, 
							const char *initial_text, 
							BMessage *message,
							uint32 rmask,
							uint32 flags)
:	BControl(frame, name, label, message, rmask, flags)
{
	// initial_text zuweisen!
	font_height	hoehe;
	GetFontHeight(&hoehe);
	ResizeTo( Bounds().Width(), hoehe.ascent + hoehe.descent + hoehe.leading + 6.0 );
	
	frame = Bounds();
	frame.left = 50.0;
	
	fTextView = new LiveTextView(frame, 0, frame.InsetByCopy(6.0, 3.0).OffsetToCopy( 3.0, 3.0 ), B_FOLLOW_LEFT|B_FOLLOW_TOP );
	AddChild( fTextView );
}

LiveTextControl::~LiveTextControl() {
}

void LiveTextControl::SetText(const char *text) {
	fTextView->SetText( text );
}

const char *LiveTextControl::Text() const {
	return fTextView->Text();
}

void LiveTextControl::SetAlignment(alignment label, alignment text) {
	fTextView->SetAlignment(text);
}

void LiveTextControl::MouseDown(BPoint where) {
	fTextView->MakeFocus();
}

void LiveTextControl::SetDivider(float dividing_line) {
	BRect bounds( Bounds() );
	bounds.left = dividing_line;
	fTextView->MoveTo( dividing_line, bounds.top );
	fTextView->ResizeTo( bounds.Width(), bounds.Height() );
};

void LiveTextControl::Draw(BRect updateRect) {
	SetHighColor( 0, 0, 0 );

	font_height	hoehe;
	GetFontHeight(&hoehe);
	DrawString(Label(), BPoint( 3.0, hoehe.ascent + 3.0 ) );
}

void LiveTextControl::MakeFocus(bool focusState) {
	if (focusState) {
		fTextView->MakeFocus();
	}
	else {
		BView::MakeFocus( false );
		BView *view;
		if (!(view = NextSibling()) && !(Parent()->NextSibling())) view = Parent();
		view->MakeFocus();
	}
}

// -------------- LiveTextView -----------------------------------

LiveTextView::LiveTextView(BRect frame,
						   const char	*name,
						   BRect		textRect,
						   uint32		resizeMask,
						   uint32		flags)
:	BTextView(frame, name, textRect, resizeMask, flags)
{
}

void LiveTextView::Draw(BRect updateRect) {

	BTextView::Draw( updateRect );

	BRect bounds( Bounds() );

	SetHighColor( 184, 184, 184 );
	StrokeLine( bounds.RightTop(), bounds.LeftTop() );
	StrokeLine( bounds.LeftBottom() );

	SetHighColor( 255, 255, 255 );
	StrokeLine( bounds.RightBottom() );
	StrokeLine( bounds.RightTop() );

	bounds.InsetBy( 1.0, 1.0 );

	if (IsFocus()) SetHighColor( 0, 0, 229 );
	else SetHighColor( 96, 96, 96);
	
	StrokeLine( bounds.RightTop(), bounds.LeftTop() );
	StrokeLine( bounds.LeftBottom() );

	if (IsFocus()) SetHighColor( 0, 0, 229 );
	else SetHighColor( 216, 216, 216 );
	StrokeLine( bounds.RightBottom() );
	StrokeLine( bounds.RightTop() );
}

void LiveTextView::MakeFocus(bool focusState) {
	BTextView::MakeFocus(focusState);
	Invalidate();
}

void LiveTextView::InsertText(const char *inText, int32 inLength, 
	int32 inOffset, const text_run_array *inRuns) {
	
	bool submit = false;
	
	if (inLength==1) {
		switch ((int)inText[0]) {
			case 10:
			case 13:
			case 9: /* TAB */
				inLength = 0;
				submit = true;
		};
	}
	if (((StringWidth(Text()) + StringWidth(inText)) > TextRect().Width() - 2.0)
		|| (inLength>0 && (strchr(inText, 10) || strchr(inText, 13) || strchr(inText, 9))))
	{
		inLength = 0;
	}
	BTextView::InsertText(inText, inLength, inOffset, inRuns);
	Draw(Bounds());
	((LiveTextControl *)Parent())->Invoke();
	if (submit) {
		Invalidate();
		Parent()->MakeFocus( false );
	}
}

void LiveTextView::DeleteText(int32 fromOffset, int32 toOffset) {
	BTextView::DeleteText(fromOffset, toOffset);
	((LiveTextControl *)Parent())->Invoke();
	Invalidate();
}
