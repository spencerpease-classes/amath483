//
// This file is part of the course materials for CSE P 524 at the University of Washington,
// Autumn 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "Timer.hpp"
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Sequential norm
double norm(const std::vector<double>& v) {
  double sum = 0.0;
  for (size_t i = 0; i < v.size(); ++i){
    sum += v[i] * v[i];
  }
  return std::sqrt(sum);
}


void randomize(std::vector<double>& v) {
  static std::default_random_engine             generator;
  static std::uniform_real_distribution<double> distribution(2.0, 32.0);
  static auto                                   dice = std::bind(distribution, generator);

  for (size_t i = 0; i < v.size(); ++i) {
    v[i] = dice();
  }
}


int main(int argc, char* argv[]) {

  size_t exponent           = 27;
  size_t num_trips          = 1;

  if (argc >= 2) exponent   = std::stol(argv[1]);
  if (argc >= 3) num_trips  = std::stol(argv[2]);

  size_t N = 1 << exponent;

  std::vector<double> A(N);
  randomize(A);

  double a = 0;

  DEF_TIMER(sequential_norm);
  START_TIMER(sequential_norm);
  for (size_t i = 0; i < num_trips; ++i) {
    a = norm(A);
  }

  double sequential_time = STOP_TIMER_QUIETLY(sequential_norm);
  std::cout << exponent << "\t" << num_trips << "\t" << sequential_time << std::endl;

  return 0;
}
