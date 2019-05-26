//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "amath583.hpp"
#include "Vector.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <iostream>


int main(int argc, char *argv[]) {

  size_t N = 1024 * 1024;

  if (argc == 2) {
    N = std::stol(argv[1]);
  }

  Vector v(N);
  randomize(v);

  double norm0 = two_norm(v);
  std::cout << "Calling two_norm first time: " << norm0 << std::endl;

  assert(norm0 == norm0);  // Is this safe / correct to do?


  double norm1 = two_norm(v);
  std::cout << "Calling two_norm second time: " << norm1 << std::endl;

  assert(norm0 == norm1);  // Is this safe / correct to do?


  // Write me: sort v in ascending order */
  double norm2 = two_norm(v);
  std::cout << "Calling two_norm with ascending values: " << norm2 << std::endl;

  //  assert(norm0 == norm2);  // Is this safe / correct to do?
  if (norm2 != norm0) {
    std::cout << "Absolute difference: " << std::abs(norm2-norm0) << std::endl;
    std::cout << "Relative difference: " << std::abs(norm2-norm0)/norm0 << std::endl;
  }


  // Write me: sort v in descending order */
  double norm3 = two_norm(v);
  std::cout << "Calling two_norm with descending values: " << norm3 << std::endl;

  assert(norm0 == norm3);  // Is this safe / correct to do?

  return 0;
}
