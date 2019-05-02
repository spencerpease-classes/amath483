

#include <random>
#include "VoVMatrix.hpp"


void   randomize(VoVMatrix& A) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto dice = std::bind(distribution, generator);
  
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = dice();
    }
  }
}


void   matmat(const VoVMatrix& A, const VoVMatrix& B, VoVMatrix& C) {
  /* WRITE ME */
}
