//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#ifndef AMATH583_BROWNBELT_AMATH583SPARSE_HPP
#define AMATH583_BROWNBELT_AMATH583SPARSE_HPP

#include <cassert>

template <typename SparseMatrixType>
void   piscetize(SparseMatrixType& A, size_t xpoints, size_t ypoints) {
  assert(A.num_rows() == A.num_cols());
  assert(xpoints * ypoints == A.num_rows());

  A.clear();

  for (size_t j = 0; j < xpoints; j++) {
    for (size_t k = 0; k < ypoints; k++) {
      size_t jrow = j * ypoints + k;

      if (j != 0) {
        size_t jcol = (j - 1) * ypoints + k;
        A.push_back(jrow, jcol, -1.0);
      }
      if (k != 0) {
        size_t jcol = j * ypoints + (k - 1);
        A.push_back(jrow, jcol, -1.0);
      }

      A.push_back(jrow, jrow, 4.0);

      if (k != ypoints - 1) {
        size_t jcol = j * ypoints + (k + 1);
        A.push_back(jrow, jcol, -1.0);
      }
      if (j != xpoints - 1) {
        size_t jcol = (j + 1) * ypoints + k;
        A.push_back(jrow, jcol, -1.0);
      }
    }
  }
}

template <typename SparseMatrixType, typename VectorType>
VectorType operator*(const SparseMatrixType& A, const VectorType& x) {
  VectorType y(A.num_rows());
  A.matvec(x, y);
  return y;
}

template <typename SparseMatrixType, typename DenseMatrixType>
DenseMatrixType operator*(const COOMatrix& A, const Matrix& B) {
  DenseMatrixType C(A.num_rows(), B.num_cols());
  A.matmat(B, C);
  return C;
}

#endif // AMATH583_BROWNBELT_AMATH583SPARSE_HPP
