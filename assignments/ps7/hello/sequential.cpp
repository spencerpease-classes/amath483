#include "Timer.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

void add(int n, float* x, float* y) {
  for (int i = 0; i < n; i++)
    y[i] = x[i] + y[i];
}

int main(int argc, char* argv[]) {

  size_t exponent  = 24;
  size_t num_trips = 1;

  if (argc >= 2) exponent = std::stol(argv[1]);
  if (argc >= 3) num_trips = std::stol(argv[2]);

  size_t N = 1 << exponent;

  float *x = (float*)malloc(N * sizeof(float)), *y = (float*)malloc(N * sizeof(float));

  for (size_t i = 0; i < N; i++)
    y[i] = 2.0 * (x[i] = 1.0f);

  DEF_TIMER(cpu_call);
  START_TIMER(cpu_call);
  for (size_t i = 0; i < num_trips; ++i) {
    add(N, x, y);
  }
  double cpu_time = STOP_TIMER_QUIETLY(cpu_call);

  free(x);
  free(y);

  std::cout << exponent << "\t" << num_trips << "\t" << cpu_time << std::endl;

  return 0;
}
