/*============================================================================
  SimpleGraphing, an object-oriented C class for graphing.
  Copyright (C) 2005-2019 by Zack T Smith.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  The author may be reached at 1@zsmith.co.
 *===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "colors.h"
#include "SimpleGraphing.h"

#define kDefaultMargin 40
#define kDefaultLeftMargin 80
#define kDefaultFontHeight (10)
#define kMaxTitleLength (200)
#define kMaxTickTextLength (40)
#define kDefaultWidth 1920
#define kDefaultHeight 1080

SimpleGraphingClass *_SimpleGraphingClass;

//----------------------------------------------------------------------------
// Name:	SimpleGraphing_draw_labels_log2
// Purpose:	Draw the labels and ticks.
//----------------------------------------------------------------------------
void
SimpleGraphing_draw_labels_log2 (SimpleGraphing* this)
{
	if (!this || !this->image)
		return;

	//----------------------------------------
	// Horizontal
	//
	// Establish min & max x values.
	//
	int i = 0;
	Value min_x = 0x4000000000000000;
	Value max_x = 0;
	for (i = 0; i < this->data_index; i += 2) {
		Value type = this->data[i];
		Value value = this->data[i+1];
		if (type == DATUM_X) {
			if (value < min_x)
				min_x = value;
			if (value > max_x)
				max_x = value;
		}
	}
	this->min_x = (long long) log2 (min_x);
	this->max_x = (long long) ceil (log2 (max_x));

	for (i = this->min_x; i <= this->max_x; i++) {
		char str [kMaxTitleLength];
		int x = this->left_margin + 
			((i-this->min_x) * this->x_span) / 
			(this->max_x - this->min_x);
		int y = this->height - this->margin + 10;

		unsigned long y2 = 1 << i;
		if (y2 < 1536) 
			snprintf (str, kMaxTitleLength-1, "%ld B", y2);
		else if (y2 < (1<<20)) {
			snprintf (str, kMaxTitleLength-1, "%ld kB", y2 >> 10);
		}
		else {
			Value j = y2 >> 20;
			switch ((y2 >> 18) & 3) {
			case 0: snprintf (str, 199, "%lld MB", (unsigned long long) j); break;
			case 1: snprintf (str, 199, "%lld.25 MB", (unsigned long long) j); break;
			case 2: snprintf (str, 199, "%lld.5 MB", (unsigned long long) j); break;
			case 3: snprintf (str, 199, "%lld.75 MB", (unsigned long long) j); break;
			}
		}

		$(this->image, draw_vline, x, y, y - 10, RGB_BLACK);
		$(this->image, draw_mini_string, str, x - 10, y + 8, RGB_BLACK);
	}

	//----------------------------------------
	// Vertical
	//
	// Establish min & max y values.
	//
	Value min_y = 0x4000000000000000;
	Value max_y = 0;
	for (i = 0; i < this->data_index; i += 2) {
		Value type = this->data[i];
		Value value = this->data[i+1];
		if (type == DATUM_Y) {
			if (value < min_y)
				min_y = value;
			if (value > max_y)
				max_y = value;
		}
	}
	this->min_y = min_y;
	this->max_y = max_y;

	int font_height = kDefaultFontHeight;
	int available_height = this->y_span;
	int max_labels = available_height / font_height;
	int preferred_n_labels = this->max_y/10000;
	int actual_n_labels;
	float multiplier = 1;
	if (preferred_n_labels < max_labels) {
		actual_n_labels = preferred_n_labels;
	} else {
		actual_n_labels = max_labels;
		multiplier = preferred_n_labels / (float) actual_n_labels;
	}

	for (i = 0; i <= actual_n_labels; i++) {
		int x = this->left_margin - 10;
		int y = this->height - this->margin - (i * this->y_span) / (float)actual_n_labels;

		$(this->image, draw_hline, x, x+10, y, RGB_BLACK);

		int value = (int) (i * multiplier);

		char str [kMaxTickTextLength];
		snprintf (str, kMaxTickTextLength-1, "%d GB/s", value);

		$(this->image, draw_mini_string, str, x - 40, y - MINIFONT_HEIGHT/2, RGB_BLACK);
	}
}

static void
SimpleGraphing_set_x_axis_mode (SimpleGraphing* this, int x_axis_mode)
{
	if (this) {
		if (x_axis_mode != MODE_X_AXIS_LINEAR && x_axis_mode != MODE_X_AXIS_LOG2)
			return;

		this->x_axis_mode = x_axis_mode;
	}
}

static void 
SimpleGraphing_clear (SimpleGraphing* this)
{
	if (this) {
		$(this->image, clear, RGB_WHITE);
		this->data_index = 0;
	}
}

static Image *SimpleGraphing_get_image (SimpleGraphing* this)
{       
        if (this) 
		return this->image;
	
	return NULL;
}

SimpleGraphing *
SimpleGraphing_init (SimpleGraphing* this)
{
	if (this) {
		this->is_a = _SimpleGraphingClass;
		this->x_axis_mode = MODE_X_AXIS_LINEAR; // Default value
		this->data_index = 0;
		this->width = 0;
		this->height = 0;
	}
	return this;
}

SimpleGraphing *
SimpleGraphing_initWithSize (SimpleGraphing* this, int width, int height)
{
	this->is_a = _SimpleGraphingClass;
	this->x_axis_mode = MODE_X_AXIS_LINEAR; // Default value

	if (width <= 0 || height <= 0) {
		width = kDefaultWidth;
		height = kDefaultHeight;
	}

	this->width = width;
	this->height = height;
	this->image = newWithSize(Image, width, height);
	this->margin = kDefaultMargin;
	this->left_margin = kDefaultLeftMargin;

	$(this->image, clear, RGB_WHITE);

	$(this->image, draw_hline, this->left_margin, this->width - this->margin, this->height - this->margin, RGB_BLACK);
	$(this->image, draw_vline, this->left_margin, this->margin, this->height - this->margin, RGB_BLACK);

	this->x_span = this->width - (this->margin + this->left_margin);
	this->y_span = this->height - 2 * this->margin;

	this->legend_y = this->margin;

	return this;
}

static void 
SimpleGraphing_set_title (SimpleGraphing* this, const char *title)
{
	if (!this || !title)
		return;

	if (this->title)
		free (this->title);
	this->title = strdup (title);

	$(this->image, draw_string, this->title, this->left_margin, this->margin/2, RGB_BLACK);
}

static void
SimpleGraphing_new_line (SimpleGraphing *this, const char *str, RGB color)
{
	if (!this || !this->image)
		return;

	$(this->image, draw_string, str, this->width - this->margin - 370, this->legend_y, 0xffffff & color);
	
	this->legend_y += 17;

	this->fg = 0;
	this->last_x = this->last_y = -1;

	if (this->data_index >= MAX_GRAPH_DATA-2) 
		return; // error ("Too many graph data.");

	this->data [this->data_index++] = DATUM_COLOR;
	this->data [this->data_index++] = color;
}

//----------------------------------------------------------------------------
// Name:	SimpleGraphing_add_point
// Purpose:	Adds a point to this list to be drawn.
//----------------------------------------------------------------------------

static void
SimpleGraphing_add_point (SimpleGraphing *this, Value x, Value y)
{
	if (!this || !this->image)
		return;

	if (this->data_index >= MAX_GRAPH_DATA-4) 
		return; // error ("Too many graph data.");

	this->data [this->data_index++] = DATUM_X;
	this->data [this->data_index++] = x;
	this->data [this->data_index++] = DATUM_Y;
	this->data [this->data_index++] = y;
}

//----------------------------------------------------------------------------
// Name:	SimpleGraphing_plot_log2
// Purpose:	Plots a point on the current graph.
//----------------------------------------------------------------------------

static void
SimpleGraphing_plot_log2 (SimpleGraphing *this, Value x, Value y)
{
	if (!this || !this->image)
		return;

	//----------------------------------------
	// Plot the point. The x axis is 
	// logarithmic, base 2.
	//
	double tmp = log2 (x);
	tmp -= (double) this->min_x;
	tmp *= (double) this->x_span;
	tmp /= (double) (this->max_x - this->min_x);

	int x2 = this->left_margin + (int) tmp;
	int y2 = this->height - this->margin - (y * this->y_span) / this->max_y;

	if (this->last_x != -1 && this->last_y != -1) {
		if (this->fg & DASHED) 
			$(this->image, draw_dashed_line, this->last_x, this->last_y, x2, y2, this->fg & 0xffffff);
		else
			$(this->image, draw_line, this->last_x, this->last_y, x2, y2, this->fg);
	}

	this->last_x = x2;
	this->last_y = y2;
}

//----------------------------------------------------------------------------
// Name:	SimpleGraphing_plot_linear
// Purpose:	Plots a point on the current graph.
//----------------------------------------------------------------------------

static void
SimpleGraphing_plot_linear (SimpleGraphing *this, Value x, Value y, Value max_y)
{
	if (!this || !this->image)
		return;

	//----------------------------------------
	// Plot the point. The x axis is 
	// logarithmic, base 2. The units of the
	// y value is kB.
	//
	double tmp = 10. + log2 (x);
	tmp -= (double) XVALUE_MIN;
	tmp *= (double) this->x_span;
	tmp /= (double) (XVALUE_MAX - XVALUE_MIN);
	int x2 = this->left_margin + (int) tmp;
	int y2 = this->height - this->margin - (y * this->y_span) / max_y;

//printf ("\tx=%d, y=%d\n",x,y); fflush(stdout);

	if (this->last_x != -1 && this->last_y != -1) {
		if (this->fg & DASHED) 
			$(this->image, draw_dashed_line, this->last_x, this->last_y, x2, y2, this->fg & 0xffffff);
		else
			$(this->image, draw_line, this->last_x, this->last_y, x2, y2, this->fg);
	}

	this->last_x = x2;
	this->last_y = y2;
}

//----------------------------------------------------------------------------
// Name:	SimpleGraphing_make_log2
// Purpose:	Plots all lines.
//----------------------------------------------------------------------------

static void
SimpleGraphing_make_log2 (SimpleGraphing *this)
{
	if (!this || !this->image)
		return;

	SimpleGraphing_draw_labels_log2 (this);

	//----------------------------------------
	// OK, now draw the lines.
	//
	int i;
	int x = -1, y = -1;
	for (i = 0; i < this->data_index; i += 2) 
	{
		Value type = this->data[i];
		Value value = this->data[i+1];

		switch (type) {
		case DATUM_Y:	y = value; break;
		case DATUM_X:	x = value; break;
		case DATUM_COLOR:	
			this->fg = (unsigned long) value; 
			this->last_x = -1;
			this->last_y = -1;
			break;
		}

		if (x != -1 && y != -1) {
			SimpleGraphing_plot_log2 (this, x, y);
			x = y = -1;
		}
	}
}

//----------------------------------------------------------------------------
// Name:	SimpleGraphing_make_linear
// Purpose:	Plots linear graph.
//----------------------------------------------------------------------------

static void
SimpleGraphing_make_linear (SimpleGraphing *this)
{
	if (!this || !this->image)
		return;

	int i;

	// No data
	if (!this->data_index)
		return;

	//----------------------------------------
	// Get the maximum bandwidth in order to
	// properly scale the graph vertically.
	//
	int max_y = 0;
	for (i = 0; i < this->data_index; i += 2) {
		if (this->data[i] == DATUM_Y) {
			int y = this->data [i+1];
			if (y > max_y)
				max_y = y;
		}
	}

	int range = max_y > 10000 ? 2 : (max_y > 1000 ? 1 : 0);
	int y_spacing = 1;
	switch (range) {
	case 2:
		// Round up to the next 100.00 MB/sec. (=10000).
		y_spacing = 10000;
		break;
	case 1:
		// Round up to the next 10.00 MB/sec. 
		y_spacing = 1000;
		break;
	case 0:
		// Round up to the next 1.00 MB/sec. 
		y_spacing = 100;
		break;
	} 
	max_y /= y_spacing;
	max_y *= y_spacing;
	max_y += y_spacing;

	//----------------------------------------
	// Draw the axes, ticks & labels.
	//
	// X axis:
	if (XVALUE_MIN < 10)
		return; // error ("Minimum y is too small.");

	for (i = XVALUE_MIN; i <= XVALUE_MAX; i++) {
		char str[kMaxTickTextLength];
		unsigned long y2 = 1 << (i-10); // XX XVALUE_MIN>=10
		if (y2 < 1024)
			snprintf (str, kMaxTickTextLength-1, "%u kB", (unsigned int) y2);
		else
			snprintf (str, kMaxTickTextLength-1, "%lu MB", (unsigned long) (y2 >> 10));

		int x = this->left_margin + ((i - XVALUE_MIN) * this->x_span) / (XVALUE_MAX - XVALUE_MIN);
		int y = this->height - this->margin + 10;
		
		$(this->image, draw_vline, x, y, y-10, RGB_BLACK);
		$(this->image, draw_mini_string, str, x - 10, y+8, RGB_BLACK);
	}

	//----------
	// Y axis:
	// Decide what the tick spacing will be.
	for (i = 0; i <= max_y; i += y_spacing) {
		char str[kMaxTickTextLength];
		unsigned long whole = i / 100;
		unsigned long frac = i % 100;
		snprintf (str, kMaxTickTextLength-1, "%lu.%02lu MB/s", whole, frac);

		int x = this->left_margin - 10;
		int y = this->height - this->margin - (i * this->y_span) / max_y;

		$(this->image, draw_hline, x, x+10, y, RGB_BLACK);
		$(this->image, draw_mini_string, str, x - 60, y - MINIFONT_HEIGHT/2, RGB_BLACK);
	}

	//----------------------------------------
	// Draw the data lines.
	//
	int x = -1, y = -1;
	this->last_x = -1;
	this->last_y = -1;
	for (i = 0; i < this->data_index; i += 2) 
	{
		int type = this->data[i];
		long value = this->data[i+1];

		switch (type) {
		case DATUM_Y:	y = value; break;
		case DATUM_X:	x = value; break;
		case DATUM_COLOR:	
			this->fg = (unsigned long) value; 
			this->last_x = -1;
			this->last_y = -1;
			break;
		}

		if (x != -1 && y != -1) {
			SimpleGraphing_plot_linear (this, x, y, max_y);
			x = y = -1;
		}
	}
}

static void
SimpleGraphing_make (SimpleGraphing *this)
{
	if (!this)
		return; // XX silent error

	switch (this->x_axis_mode) {
	case MODE_X_AXIS_LOG2:
		SimpleGraphing_make_log2 (this);
		break;
	case MODE_X_AXIS_LINEAR:
		SimpleGraphing_make_linear (this);
		break;
	default:
		fprintf (stderr, "Invalid graph mode %d.\n", this->x_axis_mode);
		break;
	}
}

void
SimpleGraphing_destroy (SimpleGraphing *this)
{
	if (!this)
		return;

	if (this->title) {
		free (this->title);
		this->title = NULL;
	}
	if (this->image) {
		delete(this->image);
		this->image = NULL;
	}
}

static void SimpleGraphing_dealloc (SimpleGraphing* this)
{
}

static void SimpleGraphing_dump (SimpleGraphing* this, FILE *file)
{
}

SimpleGraphingClass* SimpleGraphingClass_prepare ()
{
        PREPARE_CLASS_STRUCT(SimpleGraphing,Object)

        _SimpleGraphingClass->dump = SimpleGraphing_dump;
        _SimpleGraphingClass->dealloc = SimpleGraphing_dealloc;

	_SimpleGraphingClass->add_point = SimpleGraphing_add_point;
	_SimpleGraphingClass->clear = SimpleGraphing_clear;
	_SimpleGraphingClass->draw_labels_log2 = SimpleGraphing_draw_labels_log2;
	_SimpleGraphingClass->get_image = SimpleGraphing_get_image;
	_SimpleGraphingClass->make = SimpleGraphing_make;
	_SimpleGraphingClass->new_line = SimpleGraphing_new_line;
	_SimpleGraphingClass->plot_linear = SimpleGraphing_plot_linear;
	_SimpleGraphingClass->plot_log2 = SimpleGraphing_plot_log2;
	_SimpleGraphingClass->set_title = SimpleGraphing_set_title;
	_SimpleGraphingClass->set_x_axis_mode = SimpleGraphing_set_x_axis_mode;

        return _SimpleGraphingClass;
}


