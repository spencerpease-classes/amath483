//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "Matrix.hpp"
#include "Timer.hpp"
#include "amath583.hpp"
#include "amath583IO.hpp"
#include "amath583sparse.hpp"
#include <cmath>
#include <iostream>

#include "COOMatrix.hpp"
#include "CSRMatrix.hpp"

#ifdef TIME_CSC
#include "CSCMatrix.hpp"
#endif

#ifdef TIME_AOS
#include "AOSMatrix.hpp"
#endif

#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(int argc, char* argv[]) {

  Timer  t;
  size_t maxsize = 4 * 1024 * 1024;
  size_t maxrhs  = 64;
  if (argc >= 2) {
    maxsize = std::stol(argv[1]);
  }
  if (argc == 3) {
    maxrhs = std::stol(argv[2]);
  }

  std::vector<double> sizes(0);
  std::vector<double> flops(0);

  plt::figure_size(1280, 960);    // Initialize a 1280 X 960 figure

  std::cout << "================================================================" << std::endl;

  std::cout << "  Coordinate sparse matrix " << std::endl;

  for (size_t numrhs = 1; numrhs <= maxrhs; numrhs *= 4) {
    std::cout << numrhs << " right hand sides" << std::endl;

    std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
    sizes.clear();
    flops.clear();
    for (size_t size = 64; size <= maxsize; size *= 4) {
      size_t    trips = 4 + 1024 * 1024 * 1024 / (size * size);
      Matrix    x(size, numrhs), y(size, numrhs);
      COOMatrix A(size, size);
      piscetize(A, std::sqrt(size), std::sqrt(size));
      randomize(x);

      t.start();
      for (size_t trip = 0; trip <= trips; ++trip) {
        A.matmat(x, y);
      }
      t.stop();
      double f = (2.0 * numrhs * A.num_nonzeros()) / 1.E9;
      double e = (t.elapsed() / ((double)trips)) / 1.E3;
      if (e == 0.0) e = 10000;
      flops.push_back(f / e);

      sizes.push_back(size);

      std::cout << size << "\t" << trips << "\t" << e << "\t" << e / trips << "\t" << f / e << std::endl;
    }
    plt::named_loglog("COO " + std::to_string(numrhs), sizes, flops);
  }

  std::cout << "================================================================" << std::endl;
  std::cout << "  Compressed sparse row matrix " << std::endl;

  for (size_t numrhs = 1; numrhs <= maxrhs; numrhs *= 4) {
    std::cout << numrhs << " right hand sides" << std::endl;

    std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
    sizes.clear();
    flops.clear();
    for (size_t size = 64; size <= maxsize; size *= 4) {
      size_t    trips = 4 + 1024 * 1024 * 1024 / (size * size);
      Matrix    x(size, numrhs), y(size, numrhs);
      CSRMatrix A(size, size);
      piscetize(A, std::sqrt(size), std::sqrt(size));
      randomize(x);

      t.start();
      for (size_t trip = 0; trip <= trips; ++trip) {
        A.matmat(x, y);
      }
      t.stop();
      double f = (2.0 * numrhs * A.num_nonzeros()) / 1.E9;
      double e = (t.elapsed() / ((double)trips)) / 1.E3;
      if (e == 0.0) e = 10000;
      flops.push_back(f / e);

      sizes.push_back(size);

      std::cout << size << "\t" << trips << "\t" << e << "\t" << e / trips << "\t" << f / e << std::endl;
    }
    plt::named_loglog("CSR " + std::to_string(numrhs), sizes, flops);
  }

#ifdef TIME_CSC
  std::cout << "================================================================" << std::endl;
  std::cout << "  Compressed sparse column matrix " << std::endl;

  for (size_t numrhs = 1; numrhs <= maxrhs; numrhs *= 4) {
    std::cout << numrhs << " right hand sides" << std::endl;

    std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
    sizes.clear();
    flops.clear();
    for (size_t size = 64; size <= maxsize; size *= 4) {
      size_t    trips = 4 + 1024 * 1024 * 1024 / (size * size);
      Matrix    x(size, numrhs), y(size, numrhs);
      CSCMatrix A(size, size);
      piscetize(A, std::sqrt(size), std::sqrt(size));
      randomize(x);

      t.start();
      for (size_t trip = 0; trip <= trips; ++trip) {
        A.matmat(x, y);
      }
      t.stop();
      double f = (2.0 * numrhs * A.num_nonzeros()) / 1.E9;
      double e = (t.elapsed() / ((double)trips)) / 1.E3;
      if (e == 0.0) e = 10000;
      flops.push_back(f / e);

      sizes.push_back(size);

      std::cout << size << "\t" << trips << "\t" << e << "\t" << e / trips << "\t" << f / e << std::endl;
    }
    plt::named_loglog("CSC " + std::to_string(numrhs), sizes, flops);
  }
#endif    // TIME_CSC

#ifdef TIME_AOS
  std::cout << "================================================================" << std::endl;
  std::cout << "  Array of Structs sparse matrix " << std::endl;

  for (size_t numrhs = 1; numrhs <= maxrhs; numrhs *= 4) {
    std::cout << numrhs << " right hand sides" << std::endl;

    std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
    sizes.clear();
    flops.clear();
    for (size_t size = 64; size <= maxsize; size *= 4) {
      size_t    trips = 4 + 1024 * 1024 * 1024 / (size * size);
      Matrix    x(size, numrhs), y(size, numrhs);
      AOSMatrix A(size, size);
      piscetize(A, std::sqrt(size), std::sqrt(size));
      randomize(x);

      t.start();
      for (size_t trip = 0; trip <= trips; ++trip) {
        A.matmat(x, y);
      }
      t.stop();
      double f = (2.0 * numrhs * A.num_nonzeros()) / 1.E9;
      double e = (t.elapsed() / ((double)trips)) / 1.E3;
      if (e == 0.0) e = 10000;
      flops.push_back(f / e);

      sizes.push_back(size);

      std::cout << size << "\t" << trips << "\t" << e << "\t" << e / trips << "\t" << f / e << std::endl;
    }
    plt::named_loglog("AOS " + std::to_string(numrhs), sizes, flops);
  }
#endif    // TIME_AOS

  plt::xlim(64, (int)maxsize);
  plt::xlabel("Matrix Size");
  plt::ylabel("GFlops");

  plt::legend();

  plt::save("matmat_time.png");

  return 0;
}
