/*
 * Copyright 2017. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Janus
 */


/*
	Based on Tracker Modified Column
*/


#include "DateColumn.h"
#include <DateFormat.h>
#include <DateTimeFormat.h>

// #pragma mark - DateColumn


#define kTEXT_MARGIN 8


DateColumn::DateColumn(const char* title, float width, float minWidth, float maxWidth,
	alignment align)
	:
	BDateColumn(title, width, minWidth, maxWidth, align)
{
}


void
DateColumn::DrawField(BField* _field, BRect rect, BView* parent)
{
	time_t value = (int)((BDateField*)_field)->UnixTime();
	if (value <= 0)
		return;

	float width = rect.Width() - (2 * kTEXT_MARGIN);
	BDateField* field = (BDateField*)_field;

	if (field->Width() != rect.Width()) {
		float resultWidth = rect.Width() + 1;
		time_t timeValue = (time_t)value;

		// Find the longest possible format that will fit the available space
		struct {
			BDateFormatStyle dateStyle;
			BTimeFormatStyle timeStyle;
		} formats[] = {
			{B_LONG_DATE_FORMAT, B_MEDIUM_TIME_FORMAT},
			{B_LONG_DATE_FORMAT, B_SHORT_TIME_FORMAT},
			{B_MEDIUM_DATE_FORMAT, B_SHORT_TIME_FORMAT},
			{B_SHORT_DATE_FORMAT, B_SHORT_TIME_FORMAT},
		};

		BString dateString;
		BDateTimeFormat formatter;
		for (unsigned int i = 0; i < B_COUNT_OF(formats); ++i) {
			if (formatter.Format(dateString, timeValue, formats[i].dateStyle, formats[i].timeStyle)
				== B_OK) {
				resultWidth = parent->StringWidth(dateString.String(), dateString.Length());
				if (resultWidth <= width) {
					// Found a format that fits the available space, stop searching
					break;
				}
			}
		}
		if (resultWidth > width) {
			BDateFormat dateFormatter;
			if (dateFormatter.Format(dateString, timeValue, B_SHORT_DATE_FORMAT) == B_OK)
				resultWidth = parent->StringWidth(dateString.String(), dateString.Length());
		}
		if (resultWidth > width) {
			BString out_string(dateString);
			parent->TruncateString(&out_string, B_TRUNCATE_MIDDLE, width + 2);
			dateString = out_string;
		}
		field->SetClippedString(dateString);
		field->SetWidth(resultWidth);
	}
	DrawString(field->ClippedString(), parent, rect);
}
