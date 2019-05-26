/*============================================================================
  Image, an object-oriented C image manipulation class.
  Copyright (C) 2009-2019 by Zack T Smith.

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

#ifndef _IMAGE_H
#define _IMAGE_H

#include "Object.h"

#include <stdint.h>

#define BMPLIB_RELEASE "0.10"
#define BMPLIB_RELEASE_MAJOR 0
#define BMPLIB_RELEASE_MINOR 10

typedef uint32_t RGB;
typedef uint32_t RGBA;

#define FONT_HEIGHT (17)
#define MINIFONT_HEIGHT (8)

#define DECLARE_IMAGE_POLYMORPHIC_METHODS(TYPE_POINTER) \
	RGB (*get_point) (TYPE_POINTER, int, int); \
	int (*draw_mini_string) (TYPE_POINTER, const char *, int x, int y, RGB); \
	int (*draw_narrow_numbers) (TYPE_POINTER, const char *string, int x, int y, RGB color); \
	int (*draw_string) (TYPE_POINTER, const char *, int x, int y, RGB); \
	int (*mini_string_width) (TYPE_POINTER, const char *); \
	int (*string_width) (TYPE_POINTER, const char *); \
	int (*write_bmp) (const TYPE_POINTER, const char *path); \
	void (*clear) (TYPE_POINTER, RGB); \
	void (*draw_dashed_line) (TYPE_POINTER, int x0, int y0, int x1, int y1, RGB); \
	void (*draw_hline) (TYPE_POINTER, int x0, int x1, int y, RGB); \
	void (*draw_line) (TYPE_POINTER, int x0, int y0, int x1, int y1, RGB); \
	void (*draw_point) (TYPE_POINTER, int, int, RGB); \
	void (*draw_rect) (TYPE_POINTER, int x, int y, int w, int h, RGB); \
	void (*draw_vline) (TYPE_POINTER, int x, int y0, int y1, RGB); \
	void (*fill_rect) (TYPE_POINTER, int x, int y, int w, int h, RGB); 

#define DECLARE_IMAGE_CLASS_VARS 

typedef struct imageclass {
        DECLARE_OBJECT_CLASS_VARS
	DECLARE_IMAGE_CLASS_VARS 
	DECLARE_OBJECT_POLYMORPHIC_METHODS(struct image*)
	DECLARE_IMAGE_POLYMORPHIC_METHODS(struct image*)
} ImageClass;

extern ImageClass *_ImageClass;

#define DECLARE_IMAGE_INSTANCE_VARS(TYPE_POINTER) \
	int width, height; \
	RGB *pixels;

typedef struct image {
	ImageClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct image*)
	DECLARE_IMAGE_INSTANCE_VARS(struct image*)
} Image;

extern Image* Image_init (Image* object);
extern Image* Image_initWithSize (Image* object, int width, int height);
extern ImageClass* ImageClass_prepare ();

#endif

