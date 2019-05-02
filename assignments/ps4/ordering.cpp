//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//



#include <cmath>
#include <iostream>
#include "Matrix.hpp"
#include "Timer.hpp"
#include "amath583.hpp"

#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


int main(int argc, char* argv[]) {

  Timer t;
  size_t maxsize = 512;
  if (argc == 2) {
    maxsize = std::stol(argv[1]);
  }

  std::vector<double> sizes(0);
  std::vector<double> flops(0);


  plt::figure_size(1280, 960);  // Initialize a 1280 X 960 figure


  std::cout << "================================================================" << std::endl;
  std::cout << "  ijk ordering " << std::endl;
  std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
  sizes.clear();
  flops.clear();
  for (size_t size = 16; size <= maxsize; size *= 2) {
    size_t trips = 2 +  1024 * 1024 * 1024 / (size * size * size);
    Matrix A(size, size), B(size, size), C(size, size);
    randomize(A); randomize(B); randomize(C);

    t.start();
    for (size_t trip = 0; trip <= trips; ++trip) {
      matmat(A, B, C);
    }
    t.stop();
    double f = (2.0 * size * size * size) / 1.E9;
    double e = ( t.elapsed()/((double) trips) ) / 1.E3;
    if (e == 0.0) e = 10000;
    flops.push_back(f / e );

    sizes.push_back(size);

    std::cout << size << "\t" << trips << "\t" << e << "\t" << e/trips << "\t" << f/e << std::endl;
  }
  plt::named_loglog("ijk", sizes, flops);

  std::cout << "================================================================" << std::endl;
  std::cout << "  ikj ordering " << std::endl;
  std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
  sizes.clear();
  flops.clear();
  for (size_t size = 16; size <= maxsize; size *= 2) {
    size_t trips = 2 +  1024 * 1024 * 1024 / (size * size * size);
    Matrix A(size, size), B(size, size), C(size, size);
    randomize(A); randomize(B); randomize(C);

    t.start();
    for (size_t trip = 0; trip <= trips; ++trip) {
      /* call ikj here */
    }
    t.stop();
    double f = (2.0 * size * size * size) / 1.E9;
    double e = ( t.elapsed()/((double) trips) ) / 1.E3;
    if (e == 0.0) e = 10000;
    flops.push_back(f / e );

    sizes.push_back(size);

    std::cout << size << "\t" << trips << "\t" << e << "\t" << e/trips << "\t" << f/e << std::endl;
  }
  plt::named_loglog("ikj", sizes, flops);


  std::cout << "================================================================" << std::endl;
  std::cout << "  jki ordering " << std::endl;
  std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
  sizes.clear();
  flops.clear();
  for (size_t size = 16; size <= maxsize; size *= 2) {
    size_t trips = 2 +  1024 * 1024 * 1024 / (size * size * size);
    Matrix A(size, size), B(size, size), C(size, size);
    randomize(A); randomize(B); randomize(C);

    t.start();
    for (size_t trip = 0; trip <= trips; ++trip) {
      /* call jki here */
    }
    t.stop();
    double f = (2.0 * size * size * size) / 1.E9;
    double e = ( t.elapsed()/((double) trips) ) / 1.E3;
    if (e == 0.0) e = 10000;
    flops.push_back(f / e );

    sizes.push_back(size);

    std::cout << size << "\t" << trips << "\t" << e << "\t" << e/trips << "\t" << f/e << std::endl;
  }
  plt::named_loglog("jki", sizes, flops);


  plt::xlim(16, (int) maxsize);
  plt::xlabel("Matrix Size");
  plt::ylabel("GFlops");

  plt::legend();

  plt::save("ordering.png");

  return 0;
}


