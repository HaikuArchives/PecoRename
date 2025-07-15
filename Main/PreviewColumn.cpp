/*
 * Copyright 2017. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Janus
 */


#include "PreviewColumn.h"

// #pragma mark - PreviewField


PreviewField::PreviewField(const char* string)
	:
	BStringField(string),
	fError(false)
{
}


bool
PreviewField::Error() const
{
	return fError;
}


void
PreviewField::SetError(bool error)
{
	fError = error;
}


// #pragma mark - PreviewColumn


PreviewColumn::PreviewColumn(const char* title, float width, float minWidth, float maxWidth,
	uint32 truncate, alignment align)
	:
	BStringColumn(title, width, minWidth, maxWidth, align)
{
}


bool
PreviewColumn::AcceptsField(const BField* field) const
{
	return static_cast<bool>(dynamic_cast<const PreviewField*>(field));
}


void
PreviewColumn::DrawField(BField* _field, BRect rect, BView* parent)
{
	PreviewField* field = static_cast<PreviewField*>(_field);
	rgb_color color = parent->HighColor();

	if (field->Error()) {
		rgb_color red = ui_color(B_FAILURE_COLOR);
		parent->SetHighColor(red);
	}

	BStringColumn::DrawField(_field, rect, parent);
	parent->SetHighColor(color);
}
