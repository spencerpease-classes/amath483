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
#include <algorithm>
#include <iterator>

/*
// Test Sorting
void print_range(Vector& x) {

  std::cout << "First 5 elements: ";

  for (size_t i = 0; i < 5; i++) {
    std::cout << x(i) << " ";
  }

  std::cout << std::endl;

  std::cout << "Last 5 elements: ";

  for (size_t i = x.num_rows() - 5 - 1; i < x.num_rows(); i++){
    std::cout << x(i) << " ";
  }

  std::cout << std::endl;

}
*/


int main(int argc, char *argv[]) {

  size_t N = 1024 * 1024;
  size_t numthreads = 1;

  if (argc >= 2) {
    N = std::stol(argv[1]);
  }
  if (argc >= 3) {
    numthreads = std::stol(argv[2]);
  }

  Vector v(N);
  randomize(v);

  //print_range(v);

  double norm0 = two_norm(v);
  std::cout << "Calling two_norm first time: " << norm0 << std::endl;

  assert(norm0 == norm0);  // Is this safe / correct to do?


  double norm1 = two_norm(v);
  std::cout << "Calling two_norm second time: " << norm1 << std::endl;

  assert(norm0 == norm1);  // Is this safe / correct to do?


  // Write me: sort v in ascending order */
  std::sort(& v(0), & v(v.num_rows()));
  // print_range(v);
  double norm2 = two_norm(v);
  std::cout << "Calling two_norm with ascending values: " << norm2 << std::endl;

  // assert(norm0 == norm2);  // Is this safe / correct to do?
  if (norm2 != norm0) {
    std::cout << "Absolute difference: " << std::abs(norm2-norm0) << std::endl;
    std::cout << "Relative difference: " << std::abs(norm2-norm0)/norm0 << std::endl;
  }


  // Write me: sort v in descending order */
  // std::sort(& v(0), & v(v.num_rows()), [&v](auto& a, auto& b) -> bool { return v(a) > v(b); });
  std::reverse(& v(0), & v(v.num_rows()));
  // print_range(v);
  double norm3 = two_norm(v);
  std::cout << "Calling two_norm with descending values: " << norm3 << std::endl;

  // assert(norm0 == norm3);  // Is this safe / correct to do?
  if (norm2 != norm0) {
    std::cout << "Absolute difference: " << std::abs(norm3-norm0) << std::endl;
    std::cout << "Relative difference: " << std::abs(norm3-norm0)/norm0 << std::endl;
  }


  double norm4 = partitioned_two_norm(v, numthreads);
  std::cout << "Calling partitioned_two_norm with " <<  numthreads << " threads: " << norm4 << std::endl;

  // assert(norm3 == norm4);
  if (norm4 != norm3) {
    std::cout << "Absolute difference (norm3): " << std::abs(norm4-norm3) << std::endl;
    std::cout << "Relative difference (norm3): " << std::abs(norm4-norm3)/norm3 << std::endl;
  }

  if (norm4 != norm0) {
    std::cout << "Absolute difference (norm0): " << std::abs(norm4-norm0) << std::endl;
    std::cout << "Relative difference (norm0): " << std::abs(norm4-norm0)/norm0 << std::endl;
  }

  return 0;
}
