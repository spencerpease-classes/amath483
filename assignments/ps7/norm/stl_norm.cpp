
#include "Timer.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>


int main(int argc, char* argv[]) {

  size_t exponent           = 27;
  size_t num_trips          = 1;

  if (argc >= 2) exponent   = std::stol(argv[1]);
  if (argc >= 3) num_trips  = std::stol(argv[2]);

  size_t num_elements = 1 << exponent;
  
  std::vector<float> x(num_elements);
  std::fill(x.begin(), x.end(), 1.0);

  DEF_TIMER(stl_norm);
  START_TIMER(stl_norm);
  for (size_t i = 0; i < num_trips; ++i) {
    float s_norm = std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), 0.0));
  }
  double stl_time = STOP_TIMER_QUIETLY(stl_norm);

  std::cout << exponent << "\t" << num_trips << "\t" << stl_time << std::endl;

  return 0;
}
