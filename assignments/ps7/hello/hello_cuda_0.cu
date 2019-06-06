#include "Timer.hpp"
#include <iostream>
#include <string>

__global__ void add(int n, float* x, float* y) {
  for (int i = 0; i < n; i++)
    y[i] = x[i] + y[i];
}

int main(int argc, char* argv[]) {

  size_t exponent  = 24;
  size_t num_trips = 1;

  if (argc >= 2) exponent = std::stol(argv[1]);
  if (argc >= 3) num_trips = std::stol(argv[2]);

  size_t N = 1 << exponent;

  float *x = nullptr, *y = nullptr;

  cudaMallocManaged(&x, N * sizeof(float));
  cudaMallocManaged(&y, N * sizeof(float));

  for (int i = 0; i < N; i++)
    y[i] = 2.0 * (x[i] = 1.0f);

  DEF_TIMER(cuda_call);
  START_TIMER(cuda_call);
  for (size_t i = 0; i < num_trips; ++i) {
    add<<<1, 1>>>(N, x, y);
  }
  STOP_TIMER(cuda_call);

  cudaDeviceSynchronize();

  cudaFree(x);
  cudaFree(y);

  return 0;
}
