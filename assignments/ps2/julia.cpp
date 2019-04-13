//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <complex>
#include <iostream>

#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;

int scale  = 1024;
int offset = scale;

std::complex<double> xy_to_complex(int x, int y) {
  return std::complex<double>((x - scale) / ((double)scale), (y - scale) / ((double)scale));
}

std::complex<double> f(std::complex<double> x) { return /* WRITE ME (not zero) */ 0.0; }

std::complex<double> fp(std::complex<double> x) { return /* WRITE ME (not one) */ 1.0; }

int newt(std::complex<double> x0) {
  for (int i = 0; i < 256; ++i) {
    std::complex<double> delta = -f(x0) / fp(x0);
    if (std::abs(delta) < 0.00001) {
      return i;
    }
    x0 += delta;
  }

  return 255;
}

int main() {
  int width  = 1024;
  int height = 1024;

  scale = width / 2;

  int                 depth  = 1;
  int                 planes = 3;
  unsigned char       white  = 255;
  CImg<unsigned char> julia(width, height, depth, planes, white);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      std::complex<double> x   = xy_to_complex(i, j);     // convert screen coord to complex number
      unsigned char        pix = (unsigned char)newt(x);  // get number of iterations
      julia(i, j, 0, 0)        = 13 * pix;  // red
      julia(i, j, 0, 1)        = 17 * pix;  // green 
      julia(i, j, 0, 2)        = 21 * pix;  // blue
    }
  }

  julia.save_bmp("julia.bmp");

  return 0;
}
