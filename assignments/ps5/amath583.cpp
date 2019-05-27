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
#include <functional>
#include <random>
#include <thread>

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

void randomize(Vector& x) {
  std::default_random_engine             generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto                            dice = std::bind(distribution, generator);

  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = dice();
  }
}

double one_norm(const Vector& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += std::abs(x(i));
  }
  return sum;
}

double two_norm(const Vector& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double inf_norm(const Vector& x);
double p_norm(const Vector& x);

double partitioned_two_norm(const Vector& x, size_t partitions) {

  size_t partition_size = x.num_rows() / partitions;
  std::vector<std::thread> threads;
  Vector partial_sumSquares(partitions);
  zeroize(partial_sumSquares);

  for (size_t k = 0; k < partitions; k++) {

    threads.push_back(std::thread(
      [partition_size, k, &x, &partial_sumSquares]() -> void {

        for (size_t i = k * partition_size; i < (k+1) * partition_size; i++) {
          partial_sumSquares(k) += x(i) * x(i);
        }

      }
    ));
  }

  for (size_t k = 0; k < partitions; k++) {
    threads[k].join();
  }

  double sumSquares = 0.0;
  for (size_t i = 0; i < partial_sumSquares.num_rows(); i++) {
    sumSquares += partial_sumSquares(i);
  }

  double rootSumSquares = std::sqrt(sumSquares);
  return rootSumSquares;
}

Vector abs(const Vector& x) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = abs(x(i));
  }
  return y;
}

Vector operator*(double alpha, const Vector& x) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = alpha * x(i);
  }
  return y;
}

Vector operator+(const Vector& x, double alpha) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = x(i) + alpha;
  }
  return y;
}

Vector operator+(const Vector& x, const Vector& y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) + y(i);
  }
  return z;
}

Vector operator-(const Vector& x, const Vector& y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) - y(i);
  }
  return z;
}

Vector& operator*=(double alpha, const Vector& x);
Vector& operator+=(const Vector& x, const Vector& y);
Vector& operator-=(const Vector& x, const Vector& y);

// ----------------------------------------------------------------
//
// Matrix arithmetic
//
// ----------------------------------------------------------------
void zeroize(Matrix& A) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = 0.0;
    }
  }
}

void randomize(Matrix& A) {
  std::default_random_engine             generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto                            dice = std::bind(distribution, generator);

  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = dice();
    }
  }
}

void piscetize(Matrix& A, size_t xpoints, size_t ypoints) {
  assert(A.num_rows() == A.num_cols());
  assert(xpoints * ypoints == A.num_rows());

  zeroize(A);

  for (size_t j = 0; j < xpoints; j++) {
    for (size_t k = 0; k < ypoints; k++) {
      size_t jrow = j * ypoints + k;

      if (j != 0) {
        size_t jcol   = (j - 1) * ypoints + k;
        A(jrow, jcol) = -1.0;
      }
      if (k != 0) {
        size_t jcol   = j * ypoints + (k - 1);
        A(jrow, jcol) = -1.0;
      }
      A(jrow, jrow) = 4.0;

      if (k != ypoints - 1) {
        size_t jcol   = j * ypoints + (k + 1);
        A(jrow, jcol) = -1.0;
      }
      if (j != xpoints - 1) {
        size_t jcol   = (j + 1) * ypoints + k;
        A(jrow, jcol) = -1.0;
      }
    }
  }
}

double one_norm(const Matrix& A);
double two_norm(const Matrix& A);
double inf_norm(const Matrix& A);

double Frobenius_norm(const Matrix& A) {
  double result = 0.0;
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      result += A(i, j) * A(i, j);
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
  for (size_t i = 0; i < F.num_rows(); ++i) {
    for (size_t j = 0; j < F.num_cols(); ++j) {
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

void matvec(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(i) += A(i, j) * x(j);
    }
  }
}

/* Definition of matvec_ji goes here (583) */
void matvec_ij(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(i) += A(i, j) * x(j);
    }
  }
}

/* Definition of matvec_ji goes here (583) */
void matvec_ji(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t j = 0; j < A.num_cols(); ++j) {
    for (size_t i = 0; i < A.num_rows(); ++i) {
      y(i) += A(i, j) * x(j);
    }
  }
}

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

void matmat(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t j = 0; j < C.num_cols(); ++j) {
      for (size_t k = 0; k < A.num_cols(); ++k) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

/* Definition of matmat_ikj goes here */
void matmat_ikj(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t k = 0; k < A.num_cols(); ++k) {
      for (size_t j = 0; j < C.num_cols(); ++j) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

/* Definition of matmat_jki goes here */
void matmat_jki(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t j = 0; j < C.num_cols(); ++j) {
    for (size_t k = 0; k < A.num_cols(); ++k) {
      for (size_t i = 0; i < C.num_rows(); ++i) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}
