#include "Timer.hpp"
#include <iostream>
#include <string>

__global__ void add(int n, const float* x, float* y) {
  int index  = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (int i = index; i < n; i += stride)
    y[i] = x[i] + y[i];
}

int main(int argc, char* argv[]) {

  size_t exponent   = 24;
  size_t num_trips  = 1;
  int    block_size = 256;

  if (argc >= 2) exponent = std::stol(argv[1]);
  if (argc >= 3) num_trips = std::stol(argv[2]);
  if (argc >= 4) block_size = std::stol(argv[3]);

  size_t N = 1 << exponent;

  float *x = nullptr, *y = nullptr;

  cudaMallocManaged(&x, N * sizeof(float));
  cudaMallocManaged(&y, N * sizeof(float));

  for (int i = 0; i < N; i++)
    y[i] = 2.0 * (x[i] = 1.0f);

  int num_blocks = (N + block_size - 1) / block_size;

  DEF_TIMER(cuda_call);
  START_TIMER(cuda_call);
  cudaDeviceSynchronize();
  for (size_t i = 0; i < num_trips; ++i) {
    add<<<num_blocks, block_size>>>(N, x, y);
    cudaDeviceSynchronize();
  }
  double gpu_time = STOP_TIMER_QUIETLY(cuda_call);

  std::cout << exponent << "\t" << num_trips << "\t" << gpu_time << std::endl;

  cudaFree(x);
  cudaFree(y);
  return 0;
}
