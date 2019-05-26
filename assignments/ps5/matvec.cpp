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
#include "Vector.hpp"
#include "amath583.hpp"
#include <cmath>
#include <future>
#include <iostream>
#include <string>
#include <vector>

#define WITHOUT_NUMPY
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;


int main(int argc, char* argv[]) {
  size_t problem_size        = 8192 * 2;
  size_t maxthreads          = 8;
  size_t maxtrips            = 8;
  if (argc >= 2) problem_size = std::stol(argv[1]);
  if (argc >= 3) maxthreads  = std::stol(argv[2]);
  if (argc >= 4) maxtrips    = std::stol(argv[3]);

  if (problem_size % maxthreads != 0) {
    std::cout << "problem size must be multiple of partitions" << std::endl;
    return -2;
  }

  Vector x(problem_size), y(problem_size), z(problem_size);
  Matrix A(problem_size, problem_size);
  randomize(A);
  randomize(x);

  plt::figure_size(1280, 960);    // Initialize a 1280 X 960 figure
  std::vector<double> sizes(0);
  std::vector<double> times(0);


  zeroize(z);
  matvec(A, x, z);
  for (size_t numthreads = 1; numthreads <= maxthreads; numthreads *= 2) {
    zeroize(y);
    par_matvec(A, x, y, numthreads );
    double checknorm = two_norm(y - z)/two_norm(y);
    if (checknorm > 1.e-9) {
      std::cout << "Error: " << checknorm << " @  " << numthreads << " threads" << std::endl;
      return -1;
    }
  }

  DEF_TIMER(seq_matvec);
  START_TIMER(seq_matvec);  
  for (size_t trip = 0; trip < maxtrips; ++trip)
    matvec(A, x, y);
  STOP_TIMER(seq_matvec);  
  double base = t_seq_matvec.elapsed();

  sizes.clear();
  times.clear();

  for (size_t numthreads = 1; numthreads <= maxthreads; numthreads *= 2) {
    DEF_TIMER(par_matvec);
    START_TIMER(par_matvec);  
    for (size_t trip = 0; trip < maxtrips; ++trip)
      par_matvec(A, x, y, numthreads);
    STOP_TIMER(par_matvec);  
    sizes.push_back(numthreads);
    times.push_back(t_par_matvec.elapsed());
  }
  
  plt::xlim(1, (int) maxthreads);
  plt::xlabel("Number of threads");
  plt::ylabel("Parallel Speedup");

  for (size_t i = 0; i < times.size(); ++i) {
    times[i] = base / times[i];
  }

  plt::loglog(sizes, times);
  plt::save("par_matvec.png");

  return 0;
}
