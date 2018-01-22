/*
 * Copyright 2017. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Janus
 */
#ifndef DATE_COLUMN_H
#define DATE_COLUMN_H

#include <ColumnTypes.h>

class DateColumn : public BDateColumn {
public:
			DateColumn(const char* title, float width, float minWidth,
				float maxWidth,	alignment align = B_ALIGN_LEFT);

  void		DrawField(BField* field, BRect rect, BView* parent);
};

#endif	// DATE_COLUMN_H
