#include "PreviewColumn.h"

#include <View.h>

#include <stdio.h>


PreviewField::PreviewField(const char* string)
	:
	BStringField(string),
	fError(false)
{
}


void
PreviewField::SetError(bool error)
{
	fError = error;
}


bool
PreviewField::Error() const
{
	return fError;
}


// #pragma mark - PreviewColumn


PreviewColumn::PreviewColumn(const char* title, float width, float minWidth,
	float maxWidth, uint32 truncate, alignment align)
	:
	BStringColumn(title, width, minWidth, maxWidth, align)
{
}


void
PreviewColumn::DrawField(BField* _field, BRect rect, BView* parent)
{
	PreviewField* field = static_cast<PreviewField*>(_field);
    rgb_color color = parent->HighColor();
    if (field->Error()) {
    	rgb_color red = ui_color(B_FAILURE_COLOR); // make_color(255,0,0,0);
    	parent->SetHighColor(red);
    }
    BStringColumn::DrawField(_field, rect, parent);
    parent->SetHighColor(color);
}


bool
PreviewColumn::AcceptsField(const BField *field) const
{
	return static_cast<bool>(dynamic_cast<const PreviewField*>(field));
}

