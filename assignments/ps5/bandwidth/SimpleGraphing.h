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

#ifndef _SIMPLEGRAPHING_H
#define _SIMPLEGRAPHING_H

#include "Object.h"

#include <stdint.h>

typedef uint32_t RGB;
typedef uint32_t RGBA;

#define FONT_HEIGHT (17)
#define MINIFONT_HEIGHT (8)

#ifdef __WIN32__
typedef char bool;
enum {
	true=1,
	false=0
};
#else
#include <stdbool.h>
#endif
#include <stdint.h>

#include "Image.h"

typedef uint32_t Coordinate;
typedef uint64_t Value;

enum {
	MODE_X_AXIS_LINEAR = 0,
	MODE_X_AXIS_LOG2 = 1,
};

#define SIMPLE_GRAPHING_RELEASE "0.4" // OOC

#define XVALUE_MIN (15)
#define XVALUE_MAX (28)

enum {
	DATUM_X=0,
	DATUM_Y=1,
	DATUM_COLOR=2,
};

#define DECLARE_SIMPLEGRAPHING_POLYMORPHIC_METHODS(TYPE_POINTER) \
	void (*clear) (TYPE_POINTER); \
	void (*set_title) (TYPE_POINTER, const char *); \
	void (*draw_labels_log2) (TYPE_POINTER); \
	void (*new_line) (TYPE_POINTER, const char *str, RGB color); \
	void (*add_point) (TYPE_POINTER, Value x, Value y); \
	void (*plot_log2) (TYPE_POINTER, Value x, Value y); \
	void (*plot_linear) (TYPE_POINTER, Value x, Value y, Value max_amt); \
	void (*make) (TYPE_POINTER); \
	Image* (*get_image) (TYPE_POINTER); \
	void (*set_x_axis_mode) (TYPE_POINTER, int x_axis_mode); 

#define DECLARE_SIMPLEGRAPHING_CLASS_VARS

typedef struct simplegraphingclass {
        DECLARE_OBJECT_CLASS_VARS
	DECLARE_SIMPLEGRAPHING_CLASS_VARS
	DECLARE_OBJECT_POLYMORPHIC_METHODS(struct simplegraphing*)
	DECLARE_SIMPLEGRAPHING_POLYMORPHIC_METHODS(struct simplegraphing*)
} SimpleGraphingClass;

extern SimpleGraphingClass *_SimpleGraphingClass;

#define MAX_GRAPH_DATA 50000
#define DASHED 0x1000000 // dashed line flag

#define DECLARE_SIMPLEGRAPHING_INSTANCE_VARS(TYPE_POINTER) \
	Image *image; \
	char *title; \
	unsigned char x_axis_mode; \
	Coordinate width; \
	Coordinate height; \
	Coordinate left_margin; \
	Coordinate margin; \
	Coordinate last_x; \
	Coordinate last_y; \
	Coordinate x_span; \
	Coordinate y_span; \
	Coordinate legend_y; \
	RGB fg; \
	Value data [MAX_GRAPH_DATA]; \
	int data_index; \
	Value max_y; \
	Value min_y; \
	Value min_x; \
	Value max_x; 

typedef struct simplegraphing {
	SimpleGraphingClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct image*)
	DECLARE_SIMPLEGRAPHING_INSTANCE_VARS(struct image*)
} SimpleGraphing;

extern SimpleGraphing* SimpleGraphing_init (SimpleGraphing*);
extern SimpleGraphing* SimpleGraphing_initWithSize (SimpleGraphing*, int width, int height);
extern SimpleGraphingClass* SimpleGraphingClass_prepare ();

#endif
