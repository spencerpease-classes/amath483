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
#include <cmath>


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

void   randomize(Vector& x);
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


Vector operator*(double alpha, const Vector& x) {
  /* WRITE ME */
}


Vector operator+(const Vector& x, const Vector& y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) + y(i);
  }
  return z;
}


Vector operator-(const Vector& x, const Vector& y);


Vector& operator*=(double alpha, Vector& x);


Vector& operator+=(Vector& x, const Vector& y) {
  /* WRITE ME */
}


Vector& operator-=(const Vector& x, const Vector& y);



// ----------------------------------------------------------------
//
// Matrix arithmetic
//
// ----------------------------------------------------------------
void   zeroize(Matrix& x);
void   randomize(Matrix& x);
double one_norm(const Matrix& x);
double two_norm(const Matrix& x);
double inf_norm(const Matrix& x);
double Frobenius_norm(const Matrix& x);

Matrix operator+(const Matrix& x, const Matrix& y);
Matrix operator-(const Matrix& x, const Matrix& y);

Matrix& operator*=(double alpha, const Matrix& x);
Matrix& operator+=(const Matrix& x, const Matrix& y);
Matrix& operator-=(const Matrix& x, const Matrix& y);



// ----------------------------------------------------------------
//
// Matrix-vector operations
//
// ----------------------------------------------------------------
Vector operator*(const Matrix& A, const Vector& x) {
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



// ----------------------------------------------------------------
//
// Matrix-matrix operations
//
// ----------------------------------------------------------------
Matrix operator*(const Matrix& A, const Matrix& B);
void   matmat(const Matrix& A, const Vector& B, Matrix& C);


