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
#include <cmath>


void zeroize(Vector& x) {

  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = 0.0;
  }

  return;
}

double one_norm(const Vector& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += std::abs(x(i));
  }
  return sum;
}

double two_norm(const Vector& x) {
  double result = 0.0;
  /* WRITE ME */
  return result;
}

double inf_norm(const Vector& x) {
  double result = 0.0;
  /* WRITE ME */
  return result;
}

// AMATH 583 students will need to add dot()
