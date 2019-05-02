//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH_583_GREEN_BELT_VOVMATRIX_HPP
#define AMATH_583_GREEN_BELT_VOVMATRIX_HPP

#include <vector>

class VoVMatrix {
public:
  VoVMatrix(size_t M, size_t N) : num_rows_(M), num_cols_(N), storage_( /* Initialize Me */ ) {}

  double& operator()(size_t i, size_t j)       { /* Write me */ ; }
  const double& operator()(size_t i, size_t j) const { /* Write me */ ; }

  size_t num_rows() const { return num_rows_; }
  size_t num_cols() const { return num_cols_; }

private:
  size_t                           num_rows_, num_cols_;
  /* Give me a type */ storage_;
};

#endif    // AMATH_583_GREEN_BELT_VOVMATRIX_HPP
