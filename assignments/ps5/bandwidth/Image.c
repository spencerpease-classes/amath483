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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Image.h"

#include "font.h"
#include "minifont.h"

// Narrowest possible numbers.
static char* narrow_nums [] = 
{
	" # ",
	"# #",
	"# #",
	"# #",
	"# #",
	"# #",
	" # ",

	" #",
	"##",
	" #",
	" #",
	" #",
	" #",
	" #",

	" # ",
	"# #",
	"  #",
	" ##",
	"#  ",
	"#  ",
	"###",

	"###",
	"  #",
	" # ",
	"## ",
	"  #",
	"# #",
	" # ",

	"# #",
	"# #",
	"# #",
	"###",
	"  #",
	"  #",
	"  #",

	"###",
	"#  ",
	"## ",
	"  #",
	"  #",
	"# #",
	" # ",


	" # ",
	"#  ",
	"#  ",
	"## ",
	"# #",
	"# #",
	" # ",

	"###",
	"  #",
	"  #",
	" # ",
	" # ",
	" # ",
	" # ",

	" # ",
	"# #",
	"# #",
	" # ",
	"# #",
	"# #",
	" # ",

	" # ",
	"# #",
	"# #",
	" ##",
	"  #",
	" # ",
	"#  ",

	" ",
	"",
	"",
	" ",
	"",
	"",
	"#",
};

ImageClass *_ImageClass = NULL;

static void 
Image_dealloc (Image* this)
{
	if (this) {
		if (this->pixels)
			free (this->pixels);
	}
}

static void 
Image_dump (Image* this, FILE *file)
{
	if (this) {
		fprintf (file ?: stdout, "%s: %dx%d\n", this->is_a->name, this->width, this->height);
	}
}

Image* Image_init (Image* this)
{
	if (this) {
		this->is_a = _ImageClass;

		// Just to be clear, this Image has no pixels.
		this->width = 0;
		this->height = 0;
		this->pixels = NULL;
	}
	return this;
}

Image* Image_initWithSize (Image* this, int width, int height)
{
	if (this) {
		this->is_a = _ImageClass;

		// RULE: Allow 0x0 image.
		if (width<1 && height<1)
			return this;
		//----------

		// RULE: Round up dimensions to be multiple of 4.
		if (width & 3) 
			width += 4 - (width & 3);
		if (height & 3) 
			height += 4 - (height & 3);

		size_t size = width * height * sizeof (long);
		if (!(this->pixels = (RGB*) malloc (size))) {
			perror ("malloc");
			free (this);
			return NULL;
		}
		memset (this->pixels, 0, size);

		this->width = width;
		this->height = height;
	}
	return this;
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_point
 * Purpose:	Writes pixel into Image.
 *-------------------------------------------------------------------------*/
static void
Image_draw_point (Image *image, int x, int y, RGB rgb)
{
	if (!image || x<0 || y<0)
		return;
	if (x >= image->width || y >= image->height)
		return;
	if (!image->pixels)
		return;
	//----------

	image->pixels[y*image->width + x] = rgb;
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_hline
 * Purpose:	Draws horizontal line.
 *-------------------------------------------------------------------------*/
static void
Image_draw_hline (Image *this, int x0, int x1, int y, RGB rgb)
{
	if (x0 > x1) {
		int tmp=x1;
		x1=x0;
		x0=tmp;
	}
	
	while (x0 <= x1) {
		Image_draw_point (this, x0++, y, rgb);
	}
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_vline
 * Purpose:	Draws vertical line.
 *-------------------------------------------------------------------------*/
static void
Image_draw_vline (Image *this, int x, int y0, int y1, RGB rgb)
{
	if (y0 > y1) {
		int tmp=y1;
		y1=y0;
		y0=tmp;
	}
	
	while (y0 <= y1) {
		Image_draw_point (this, x, y0++, rgb);
	}
}

/*---------------------------------------------------------------------------
 * Name:	Image_line_core
 * Purpose:	Draws a line in an Image.
 *-------------------------------------------------------------------------*/
static void
Image_line_core (Image *image, int x0, int y0, int x1, int y1, RGB rgb,
			int dashed)
{
	if ((rgb >> 24) == 0xff)
		return;

	int dot_counter = 0;

	if (!dashed && x0 == x1 && y0 == y1) 
		Image_draw_point (image, x0, y0, rgb);
	else if (!dashed && x0 == x1)
		Image_draw_vline (image, x0, y0, y1, rgb);
	else if (!dashed && y0 == y1)
		Image_draw_hline (image, x0, x1, y0, rgb);
	else {
		int j, x, y, dx, dy, e, xchange, s1, s2;

		// DDA, copied from my FramebufferUI project.

		x = x0;
		y = y0;
		s1 = 1;
		s2 = 1;

		dx = x1 - x0;
		if (dx < 0) {
			dx = -dx;
			s1 = -1;
		}

		dy = y1 - y0;
		if (dy < 0) {
			dy = -dy;
			s2 = -1;
		}

		xchange = 0;

		if (dy > dx) {
			int tmp = dx;
			dx = dy;
			dy = tmp;
			xchange = 1;
		}

		e = (dy<<1) - dx;
		j = 0;

		while (j <= dx) {
			j++;

			int draw = 1;
			if (dashed && (1 & (dot_counter >> 2))) 
				draw = 0;
			
			if (draw)
				Image_draw_point (image, x, y, rgb);

			dot_counter++;

			if (e >= 0) {
				if (xchange)
					x += s1;
				else
					y += s2;
				e -= (dx << 1);
			}
			if (xchange) 
				y += s2;
			else
				x += s1;
			e += (dy << 1);
		}
	}
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_line
 * Purpose:	Draws a line in a Image image.
 *-------------------------------------------------------------------------*/
static void
Image_draw_line (Image *image, int x0, int y0, int x1, int y1, RGB rgb)
{
	Image_line_core (image, x0, y0, x1, y1, rgb, 0);
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_dashed_line
 * Purpose:	Draws a dashed line in a Image image.
 *-------------------------------------------------------------------------*/
static void
Image_draw_dashed_line (Image *image, int x0, int y0, int x1, int y1, RGB rgb)
{
	Image_line_core (image, x0, y0, x1, y1, rgb, 1);
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_rect
 * Purpose:	Fills a rectangle with a color.
 *-------------------------------------------------------------------------*/
static void
Image_draw_rect (Image *this, int x, int y, int w, int h, RGB rgb)
{
	if (!this)
		return;

	Image_draw_hline (this, x, x+w-1, y, rgb);
	Image_draw_hline (this, x, x+w-1, y+h-1, rgb);
	Image_draw_vline (this, x, y, y+h-1, rgb);
	Image_draw_vline (this, x+w-1, y, y+h-1, rgb);
}

/*---------------------------------------------------------------------------
 * Name:	Image_fill_rect
 * Purpose:	Fills a rectangle with a color.
 *-------------------------------------------------------------------------*/
static void
Image_fill_rect (Image *this, int x, int y, int w, int h, RGB rgb)
{
	if (!this)
		return;

	while (h > 0) {
		Image_draw_hline (this, x, x+w-1, y, rgb);
		h--;
		y++;
	}
}

/*---------------------------------------------------------------------------
 * Name:	Image_clear
 * Purpose:	Sets all pixels to specified color.
 *-------------------------------------------------------------------------*/
static void
Image_clear (Image *image, RGB rgb)
{
	Image_fill_rect (image, 0, 0, image->width, image->height, rgb);
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_string
 * Purpose:	Draws ature 5x8 characters into the this.
 *-------------------------------------------------------------------------*/
static int
Image_draw_string (Image *this, const char *string, int x, int y, RGB color)
{
	char ch;
	const char *s;

	if (!this || !string)
		return 0;
	if (x >= this->width || y >= this->height || !*string)
		return 0;
	//----------

#if 0
	RGB r,g,b;
	RGB light;
	RGB dark;
	r = 0xff & (color >> 16);
	g = 0xff & (color >> 8);
	b = 0xff & color;
	r += 3*0xff;
	b += 3*0xff;
	g += 3*0xff;
	r /= 4;
	g /= 4;
	b /= 4;
	light = b | (g << 8) | (r << 16);

	r = 0xff & (color >> 16);
	g = 0xff & (color >> 8);
	b = 0xff & color;
	r += 0xff;
	b += 0xff;
	g += 0xff;
	r /= 2;
	g /= 2;
	b /= 2;
	dark = b | (g << 8) | (r << 16);
#endif

	s = string;
	while ((ch = *s++)) {
		int ix = -1;
		if (ch == ' ') {
			x += 10;
			continue;
		}
		if (ch > 'z')
			continue;
		if (ch > ' ' && ch <= 'z')
			ix = FONT_HEIGHT * (ch - 33);
		
		if (ix >= 0) {
			int i;
			int width = 0;

			for (i=0; i<FONT_HEIGHT ; i++) {
				int j=0;
				char ch2;
				const char *s2 = font_chars[ix + i];
				int width2 = s2 ? strlen (s2) : 0;
				if (width < width2)
					width = width2;
				while ((ch2 = *s2++)) {
					RGB color_to_use;
					char draw = 1;
					switch (ch2) {
					case '#':
						color_to_use = color;
						break;
					default:
						draw = 0;
					}
					if (draw)
						Image_draw_point (this,x+j, y+i, color_to_use);
					j++;
				}
			}

			x += width + 2/* kerning */;
		}
	}

	return x;
}

/*---------------------------------------------------------------------------
 * Name:	Image_string_width
 * Purpose:	Gets width of 10x16 characters.
 *-------------------------------------------------------------------------*/
static int
Image_string_width (Image *this, const char *string)
{
	char ch;
	const char *s;
	int width = 0;

	if (!string)
		return 0;
	//----------

	s = string;
	while ((ch = *s++)) {
		int ix = -1;
		if (ch == ' ') {
			width += 10;
			continue;
		}
		if (ch > 'z')
			continue;
		if (ch > ' ' && ch <= 'z')
			ix = FONT_HEIGHT * (ch - 33);
		
		if (ix >= 0) {
			int j;
			int max_w = 0;
			for (j = 0; j < FONT_HEIGHT; j++) {
				const char *ptr = font_chars [j+ix];
				int w = ptr ? strlen (ptr) : 0;
				if (max_w < w) max_w = w;
			}

			width += max_w + 2/* kerning */;
		}
	}

	return width;
}

/*---------------------------------------------------------------------------
 * Name:	Image_draw_mini_string
 * Purpose:	Draws miniature 5x8 characters into the this.
 *-------------------------------------------------------------------------*/
static int
Image_draw_mini_string (Image *this, const char *string, int x, int y, RGB color)
{
	char ch;
	const char *s;

	if (!this || !string)
		return 0;
	if (x >= this->width || y >= this->height || !*string)
		return 0;
	//----------

#if 0
	unsigned long r,g,b;
	unsigned long light, dark;
	r = 0xff & (color >> 16);
	g = 0xff & (color >> 8);
	b = 0xff & color;
	r += 3*0xff;
	b += 3*0xff;
	g += 3*0xff;
	r /= 4;
	g /= 4;
	b /= 4;
	light = b | (g << 8) | (r << 16);

	r = 0xff & (color >> 16);
	g = 0xff & (color >> 8);
	b = 0xff & color;
	r += 0xff;
	b += 0xff;
	g += 0xff;
	r /= 2;
	g /= 2;
	b /= 2;
	dark = b | (g << 8) | (r << 16);
#endif

#define MINI_HEIGHT (8)
	s = string;
	while ((ch = *s++)) {
		int ix = -1;
		if (ch == ' ') {
			x += 5;
			continue;
		}
		if (ch > 'z')
			continue;
		if (ch > ' ' && ch <= 'z')
			ix = MINI_HEIGHT * (ch - 33);
		
		if (ix >= 0) {
			int i;

			int width = 0;
			for (i=0; i<MINI_HEIGHT; i++) {
				int j=0;
				char ch2;
				const char *s2 = mini_chars[ix + i];
				int width2 = s2 ? strlen (s2) : 0;
				if (width < width2)
					width = width2;
				while ((ch2 = *s2++)) {
					RGB color_to_use;
					char draw = 1;
					switch (ch2) {
					case '#':
						color_to_use = color;
						break;
					default:
						draw = 0;
					}
					if (draw)
						Image_draw_point (this,x+j, y+i, color_to_use);
					j++;
				}
			}

			x += width + 1/* kerning */;
		}
	}

	return x;
}

/*---------------------------------------------------------------------------
 * Name:	Image_mini_string_width
 * Purpose:	Gets width of miniature 5x8 characters.
 *-------------------------------------------------------------------------*/
static int
Image_mini_string_width (Image *this, const char *string)
{
	char ch;
	const char *s;
	int width = 0;

	if (!string)
		return 0;
	//----------

	s = string;
	while ((ch = *s++)) {
		int ix = -1;
		if (ch == ' ') {
			width += 5;
			continue;
		}
		if (ch > 'z')
			continue;
		if (ch > ' ' && ch <= 'z')
			ix = MINI_HEIGHT * (ch - 33);
		
		if (ix >= 0) {
			int max_w = 0;
			int j;
			for (j = 0; j < MINI_HEIGHT; j++) {
				const char *ptr = mini_chars [j+ix];
				int w = ptr ? strlen (ptr) : 0;
				if (max_w < w) max_w = w;
			}

			width += max_w + 1/*kerning*/;
		}
	}

	return width;
}

/*---------------------------------------------------------------------------
 * Name:	Image_narrow_numbers
 * Purpose:	Draws miniature 4x7 characters into the this.
 *-------------------------------------------------------------------------*/
static int
Image_draw_narrow_numbers (Image *this, const char *string, int x, int y, RGB color)
{
	char ch;
	const char *s;

	if (!this || !string)
		return 0;
	if (x >= this->width || y >= this->height || !*string)
		return 0;
	//----------

#define NARROW_HEIGHT (7)
	s = string;
	while ((ch = *s++)) {
		int ix = -1;
		if (ch == ' ') {
			x += 3;
			continue;
		}
		if (ch >= '0' && ch <= '9')
			ix = ch - '0';
		else
		if (ch == '.')
			ix = 10;
		
		ix *= NARROW_HEIGHT;
		
		if (ix >= 0) {
			int i;
			int width = strlen (narrow_nums [ix]);

			for (i=0; i<NARROW_HEIGHT; i++) {
				int j=0;
				char ch2;
				const char *s2 = narrow_nums [ix + i];
				while ((ch2 = *s2++)) {
					if (ch2 == '#') {
						Image_draw_point (this, 
							x+j, y+i, color);
					}
					j++;
				}
			}

			x += width + 1;
		}
	}

	return x;
}

/*---------------------------------------------------------------------------
 * Name:	Image_get_point
 * Purpose:	Reads pixel out of this.
 *-------------------------------------------------------------------------*/
static RGB
Image_get_point (Image *this, int x, int y)
{
	if (!this || x<0 || y<0)
		return 0;
	if (x >= this->width || y >= this->height)
		return 0;
	if (!this->pixels)
		return 0;
	//----------

	return this->pixels[y*this->width + x];
}

/*---------------------------------------------------------------------------
 * Name:	Image_write_bmp	
 * Purpose:	Writes this to Image file.
 *-------------------------------------------------------------------------*/
static int 
Image_write_bmp (const Image* this, const char *path)
{
	FILE *f;
#define HDRLEN (54)
	unsigned char h[HDRLEN];
	unsigned long len;
	int i, j;

	if (!this || !path)
		return -1;
	//----------

	memset (h, 0, HDRLEN);

	//--------------------
	// Create the file.
	//
	f = fopen (path, "wb");
	if (!f)
		return 0;

	//--------------------
	// Prepare header
	//
	len = HDRLEN + 3 * this->width * this->height;
	h[0] = 'B';
	h[1] = 'M';
	h[2] = len & 0xff;
	h[3] = (len >> 8) & 0xff;
	h[4] = (len >> 16) & 0xff;
	h[5] = (len >> 24) & 0xff;
	h[10] = HDRLEN;
	h[14] = 40;
	h[18] = this->width & 0xff;
	h[19] = (this->width >> 8) & 0xff;
	h[20] = (this->width >> 16) & 0xff;
	h[22] = this->height & 0xff;
	h[23] = (this->height >> 8) & 0xff;
	h[24] = (this->height >> 16) & 0xff;
	h[26] = 1;
	h[28] = 24;
	h[34] = 16;
	h[36] = 0x13; // 2835 pixels/meter
	h[37] = 0x0b;
	h[42] = 0x13; // 2835 pixels/meter
	h[43] = 0x0b;

	//--------------------
	// Write header.
	//
	if (HDRLEN != fwrite (h, 1, HDRLEN, f)) {
		fclose (f);
		return 0;
	}

	//----------------------------------------
	// Write pixels.
	// Note that Image has lower rows first.
	//
	for (j=this->height-1; j >= 0; j--) {
		for (i=0; i < this->width; i++) {
			unsigned char rgb[3];
			int ix = i + j * this->width;
			unsigned long pixel = this->pixels[ix];
			rgb[0] = pixel & 0xff;
			rgb[1] = (pixel >> 8) & 0xff;
			rgb[2] = (pixel >> 16) & 0xff;
			if (3 != fwrite (rgb, 1, 3, f)) {
				fclose (f);
				return 0;
			}
		}
	}

	fclose (f);
	return 1;
}

ImageClass* ImageClass_prepare ()
{
	PREPARE_CLASS_STRUCT(Image,Object)

	_ImageClass->dump = Image_dump;
        _ImageClass->dealloc = Image_dealloc;

	_ImageClass->clear = Image_clear;
	_ImageClass->draw_dashed_line = Image_draw_dashed_line;
	_ImageClass->draw_hline = Image_draw_hline;
	_ImageClass->draw_line = Image_draw_line;
	_ImageClass->draw_point = Image_draw_point;
	_ImageClass->draw_rect = Image_draw_rect;
	_ImageClass->draw_mini_string = Image_draw_mini_string;
	_ImageClass->draw_narrow_numbers = Image_draw_narrow_numbers;
	_ImageClass->draw_string = Image_draw_string;
	_ImageClass->draw_vline = Image_draw_vline;
	_ImageClass->fill_rect = Image_fill_rect;
	_ImageClass->get_point = Image_get_point;
	_ImageClass->write_bmp = Image_write_bmp;
	_ImageClass->string_width = Image_string_width;
	_ImageClass->mini_string_width = Image_mini_string_width;
	
	return _ImageClass;
}

