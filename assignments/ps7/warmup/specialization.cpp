

#include <iostream>
#include "Matrix.hpp"
#include "ColMatrix.hpp"
#include "RowMatrix.hpp"

template <typename MatrixType>
void multiply(const MatrixType& A, const MatrixType& B, MatrixType& C) {
  std::cout << "Generic multiply" << std::endl;
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < B.num_cols(); ++j) {
      for (size_t k = 0; k < A.num_cols(); ++k) {
	C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

template <>
void multiply(const ColMatrix& A, const ColMatrix& B, ColMatrix& C) {
  std::cout << "specialized ColMatrix multiply" << std::endl;
  for (size_t j = 0; j < B.num_cols(); ++j) {
    for (size_t k = 0; k < A.num_cols(); ++k) {
      for (size_t i = 0; i < A.num_rows(); ++i) {
	C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}


int main() {

  size_t N = 256;
  Matrix A(N, N), B(N, N), C(N, N);
  RowMatrix rA(N, N), rB(N, N), rC(N, N);
  ColMatrix cA(N, N), cB(N, N), cC(N, N);
  multiply(A, B, C);
  multiply(rA, rB, rC);
  multiply(cA, cB, cC);

  return 0;
}
