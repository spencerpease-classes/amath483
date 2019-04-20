//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//


#include <iostream>
#include "Timer.hpp"
#include <cstdlib>

int main(int argc, char* argv[]) {

  size_t loops = // FIXME

  double a = 3.14, b = 3.14159, c = 0.0;

  Timer T;
  T.start();
  for (size_t i = 0; i < loops; ++i) {
    c += a * b;
  }
  T.stop();

  std::cout << loops << " loops took " << T.elapsed() << " milliseconds" << std::endl;

  return 0;
}
