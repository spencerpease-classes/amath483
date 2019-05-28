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
#include "Vector.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include "Timer.hpp"
#include <thread>
#include <future>


#define WITHOUT_NUMPY
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;


int main(int argc, char *argv[]) {

  size_t numtrips = 256;
  size_t N = 1024 * 1024;
  size_t maxthreads = 16;
  size_t maxlevels = 4;

  if (argc >= 2) {
    N = std::stol(argv[1]);
  }
  if (argc >= 3) {
    maxthreads = std::stol(argv[2]);
  }
  if (argc >= 4) {
    maxlevels = std::stol(argv[3]);
  }

  Vector v(N);
  randomize(v);

  std::cout << "Regular ||v|| = " << two_norm(v) << std::endl;

  plt::figure_size(1280, 960);    // Initialize a 1280 X 960 figure
  std::vector<double> sizes(0);
  std::vector<double> times(0);

  sizes.clear();
  times.clear();

  for (size_t numthreads = 1; numthreads <= maxthreads; numthreads *= 2) {
    DEF_TIMER(part_norm);
    START_TIMER(part_norm);
    double part_norm = 0.0;
    for (size_t trips = 0; trips < numtrips; ++trips) {
      part_norm += partitioned_two_norm(v, numthreads);
    }
    part_norm /= numtrips;
    STOP_TIMER(part_norm);
    sizes.push_back(numthreads);
    times.push_back(t_part_norm.elapsed());
    std::cout << "||v|| = " << part_norm <<std::endl;
  }

  plt::xlim((size_t) 1, maxthreads);
  plt::xlabel("Number of threads");
  plt::ylabel("Time(ms)");

  plt::named_loglog("partitioned", sizes, times);

  sizes.clear();
  times.clear();

  size_t numlevels = 0;
  for (size_t numthreads = 1; numthreads <= maxthreads; numthreads *= 2) {
    DEF_TIMER(rec_norm);
    START_TIMER(rec_norm);
    double rec_norm = 0.0;
    for (size_t trips = 0; trips < numtrips; ++trips) {
      rec_norm += recursive_two_norm(v, numlevels);
    }
    rec_norm /= numtrips;
    STOP_TIMER(rec_norm);
    sizes.push_back(numthreads);
    times.push_back(t_rec_norm.elapsed());
    std::cout << "||v|| = " << rec_norm <<std::endl;
    ++numlevels;
  }

  plt::named_loglog("recursive", sizes, times);
  plt::legend();
  plt::save("partitioned_vs_recursive.png");

  return 0;
}
