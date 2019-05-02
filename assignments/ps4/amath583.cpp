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
#include "Matrix.hpp"
#include "Vector.hpp"
#include <cassert>
#include <random>
#include <cmath>
#include <functional>


// ----------------------------------------------------------------
//
// Vector arithmetic
//
// ----------------------------------------------------------------
void zeroize(Vector& x) {
  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = 0.0;
  }
}

void   randomize(Vector& x) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto dice = std::bind(distribution, generator);

  for (size_t i = 0; i < x.num_rows(); ++i) {
      x(i) = dice();
  }
}


double one_norm(const Vector& x);

double two_norm(const Vector& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double inf_norm(const Vector& x);
double p_norm(const Vector& x);

Vector operator+(const Vector& x, const Vector& y);
Vector operator-(const Vector& x, const Vector& y);

Vector& operator*=(double alpha, const Vector& x);
Vector& operator+=(const Vector& x, const Vector& y);
Vector& operator-=(const Vector& x, const Vector& y);



// ----------------------------------------------------------------
//
// Matrix arithmetic
//
// ----------------------------------------------------------------
void   zeroize(Matrix& A) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = 0.0;
    }
  }
}

void   randomize(Matrix& A) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto dice = std::bind(distribution, generator);

  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = dice();
    }
  }
}

double one_norm(const Matrix& A);
double two_norm(const Matrix& A);
double inf_norm(const Matrix& A);


double Frobenius_norm(const Matrix& A) {
  double result = 0.0;

  for(size_t i = 0; i < A.num_rows(); ++i) {
    for(size_t j = 0; j < A.num_cols(); ++j) {
      result += std::pow(std::abs(A(i, j)), 2);
    }
  }

  return std::sqrt(result);
}

Matrix operator+(const Matrix& A, const Matrix& B) {
  assert((A.num_rows() == B.num_rows()) && (A.num_cols() == B.num_cols()));

  Matrix C(A.num_rows(), A.num_cols());

  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t j = 0; j < C.num_cols(); ++j) {
      C(i, j) = A(i, j) + B(i, j);
    }
  }
  return C;
}


Matrix operator-(const Matrix& D, const Matrix& E) {

  assert((D.num_rows() == E.num_rows()) && (D.num_cols() == E.num_cols()));

  Matrix F(D.num_rows(), D.num_cols());

  for(size_t i = 0; i < F.num_rows(); ++i) {
    for(size_t j = 0; j < F.num_cols(); ++j) {
      F(i, j) = D(i, j) - E(i, j);
    }
  }

  return F;
}

Matrix& operator*=(double alpha, const Matrix& x);
Matrix& operator+=(const Matrix& x, const Matrix& y);
Matrix& operator-=(const Matrix& x, const Matrix& y);



// ----------------------------------------------------------------
//
// Matrix-vector operations
//
// ----------------------------------------------------------------
Vector operator*(const Matrix& A, const Vector& x) {
  assert(A.num_cols() == x.num_rows());

  Vector y(A.num_rows());
  matvec(A, x, y);
  return y;
}

void   matvec(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(i) += A(i, j) * x(j);
    }
  }
}


/* Definition of matvec_ji goes here (583) */

/* Definition of matvec_ji goes here (583) */


// ----------------------------------------------------------------
//
// Matrix-matrix operations
//
// ----------------------------------------------------------------
Matrix operator*(const Matrix& A, const Matrix& B) {
  assert(A.num_cols() == B.num_rows());

  Matrix C(A.num_rows(), B.num_cols());
  matmat(A, B, C);
  return C;
}


void   matmat(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t j = 0; j < C.num_cols(); ++j) {
      for (size_t k = 0; k < A.num_cols(); ++k) {
	C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}


/* Definition of matmat_ikj goes here */


/* Definition of matmat_jki goes here */
