//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include "Timer.hpp"
#include "Matrix.hpp"
#include "amath583.hpp"


int main() {
  Timer t;

  std::cout << "N\ttrips\ttotal\tavg" << std::endl;
  for (size_t size = 16; size <= 128; size *= 2) {
    size_t trips = 1024 * 1024 * 1024 / (size * size * size);
    Matrix A(size, size), B(size, size), C(size, size);
    randomize(A); randomize(B); randomize(C);

    t.start();
    for (size_t trip = 0; trip <= trips; ++trip) {
      matmat(A, B, C);
    }
    t.stop();
    double e = t.elapsed();
    std::cout << size << "\t" << trips << "\t" << e << "\t" << e/trips << std::endl;
  }


  return 0;
}
