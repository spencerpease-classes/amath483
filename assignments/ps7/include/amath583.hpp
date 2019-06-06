//
// This file is part of the course materials for AMATH483/583 at the University
// of Washington, Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_BROWNBELT_AMATH583_HPP
#define AMATH583_BROWNBELT_AMATH583_HPP

#include <cassert>
#include <cmath>
#include <functional>
#include <random>

// ----------------------------------------------------------------
//
// Vector arithmetic
//
// ----------------------------------------------------------------
#if (__cplusplus == 201704L)
template <typename VectorType> void zeroize(VectorType &x) {
  if constexpr (VectorType::dims == 1) {
    for (size_t i = 0; i < x.num_rows(); ++i) {
      x(i) = 0.0;
    }
  } else if constexpr (VectorType::dims == 2) {
    for (size_t i = 0; i < x.num_rows(); ++i) {
      for (size_t j = 0; j < x.num_cols(); ++j) {
        x(i, j) = 0.0;
      }
    }
  }
}

template <typename VectorType>
void randomize(VectorType &x, double scale = 1.0) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-scale, scale);
  static auto dice = std::bind(distribution, generator);

  if constexpr (VectorType::dims == 1) {
    for (size_t i = 0; i < x.num_rows(); ++i) {
      x(i) = dice();
    }
  } else if constexpr (VectorType::dims == 2) {
    for (size_t i = 0; i < x.num_rows(); ++i) {
      for (size_t j = 0; j < x.num_cols(); ++j) {
        x(i, j) = dice();
      }
    }
  }
}
#else
template <typename VectorType> void zeroize(VectorType &x);
template <typename VectorType>
void randomize(VectorType &x, double scale = 1.0);

template <> void zeroize(Vector &x) {
  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = 0.0;
  }
}

template <> void randomize(Vector &x, double scale) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-scale, scale);
  static auto dice = std::bind(distribution, generator);

  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = dice();
  }
}

template <> void randomize(Matrix &x, double scale) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-scale, scale);
  static auto dice = std::bind(distribution, generator);

  for (size_t i = 0; i < x.num_rows(); ++i) {
    for (size_t j = 0; j < x.num_cols(); ++j) {
      x(i, j) = dice();
    }
  }
}



#endif

template <typename VectorType> double one_norm(const VectorType &x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum = sum + std::abs(x(i));
  }
  return sum;
}

template <typename VectorType> double two_norm(const VectorType &x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum = sum + x(i) * x(i);
  }
  return std::sqrt(sum);
}

template <typename VectorType> VectorType abs(const VectorType &x) {
  VectorType y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = abs(x(i));
  }
  return y;
}

template <typename VectorType>
VectorType operator*(double alpha, const VectorType &x) {
  VectorType y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = alpha * x(i);
  }
  return y;
}

template <typename VectorType>
VectorType operator+(const VectorType &x, double alpha) {
  VectorType y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = x(i) + alpha;
  }
  return y;
}

template <typename VectorType>
VectorType &operator*=(double alpha, const VectorType &x);
template <typename VectorType>
VectorType &operator+=(const VectorType &x, const VectorType &y);
template <typename VectorType>
VectorType &operator-=(const VectorType &x, const VectorType &y);

// ----------------------------------------------------------------
//
// Matrix arithmetic
//
// ----------------------------------------------------------------

template <typename MatrixType>
void piscetize(MatrixType &A, size_t xpoints, size_t ypoints) {
  assert(A.num_rows() == A.num_cols());
  assert(xpoints * ypoints == A.num_rows());

  zeroize(A);

  for (size_t j = 0; j < xpoints; j++) {
    for (size_t k = 0; k < ypoints; k++) {
      size_t jrow = j * ypoints + k;

      if (j != 0) {
        size_t jcol = (j - 1) * ypoints + k;
        A(jrow, jcol) = -1.0;
      }
      if (k != 0) {
        size_t jcol = j * ypoints + (k - 1);
        A(jrow, jcol) = -1.0;
      }
      A(jrow, jrow) = 4.0;

      if (k != ypoints - 1) {
        size_t jcol = j * ypoints + (k + 1);
        A(jrow, jcol) = -1.0;
      }
      if (j != xpoints - 1) {
        size_t jcol = (j + 1) * ypoints + k;
        A(jrow, jcol) = -1.0;
      }
    }
  }
}

template <typename MatrixType> double one_norm(const MatrixType &A);
template <typename MatrixType> double two_norm(const MatrixType &A);
template <typename MatrixType> double inf_norm(const MatrixType &A);

template <typename MatrixType> double Frobenius_norm(const MatrixType &A) {
  double result = 0.0;
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      result += A(i, j) * A(i, j);
    }
  }
  return std::sqrt(result);
}

#if (__cplusplus == 201704L)

template <typename VectorType>
VectorType operator+(const VectorType &A, const VectorType &B) {
  assert(A.num_rows() == B.num_rows());

  if constexpr (VectorType::dims == 1) {
    VectorType C(A.num_rows());
    for (size_t i = 0; i < C.num_rows(); ++i) {
      C(i) = A(i) + B(i);
    }
    return C;
  } else if constexpr (VectorType::dims == 2) {
    assert(A.num_cols() == B.num_cols());
    VectorType C(A.num_rows(), A.num_cols());
    for (size_t i = 0; i < C.num_rows(); ++i) {
      for (size_t j = 0; j < C.num_cols(); ++j) {
        C(i, j) = A(i, j) + B(i, j);
      }
    }
    return C;
  }
}

template <typename VectorType>
VectorType operator-(const VectorType &A, const VectorType &B) {
  assert(A.num_rows() == B.num_rows());

  if constexpr (VectorType::dims == 1) {
    VectorType C(A.num_rows());
    for (size_t i = 0; i < C.num_rows(); ++i) {
      C(i) = A(i) - B(i);
    }
    return C;
  } else if constexpr (VectorType::dims == 2) {
    assert(A.num_cols() == B.num_cols());
    VectorType C(A.num_rows(), A.num_cols());
    for (size_t i = 0; i < C.num_rows(); ++i) {
      for (size_t j = 0; j < C.num_cols(); ++j) {
        C(i, j) = A(i, j) - B(i, j);
      }
    }
    return C;
  }
}
#else
template <typename VectorType>
VectorType operator+(const VectorType &A, const VectorType &B);
template <typename VectorType>
VectorType operator-(const VectorType &A, const VectorType &B);

template <> Vector operator+(const Vector &x, const Vector &y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) + y(i);
  }
  return z;
}

template <> Vector operator-(const Vector &x, const Vector &y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) - y(i);
  }
  return z;
}
#endif

// ----------------------------------------------------------------
//
// Matrix-vector operations
//
// ----------------------------------------------------------------
template <typename MatrixType, typename VectorType>
void matvec(const MatrixType &A, const VectorType &x, VectorType &y) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(i) += A(i, j) * x(j);
    }
  }
}

template <typename MatrixType, typename VectorType>
VectorType operator*(const MatrixType &A, const VectorType &x) {
  assert(A.num_cols() == x.num_rows());

  VectorType y(A.num_rows());
  matvec(A, x, y);
  return y;
}



// ----------------------------------------------------------------
//
// Matrix-matrix operations
//
// ----------------------------------------------------------------
template <typename MatrixType>
MatrixType operator*(const MatrixType &A, const MatrixType &B) {
  assert(A.num_cols() == B.num_rows());

  MatrixType C(A.num_rows(), B.num_cols());
  matmat(A, B, C);
  return C;
}

template <typename MatrixType>
void matmat(const MatrixType &A, const MatrixType &B, MatrixType &C) {
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t k = 0; k < A.num_cols(); ++k) {
      for (size_t j = 0; j < C.num_cols(); ++j) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

#endif // AMATH583_BROWNBELT_AMATH583_HPP
