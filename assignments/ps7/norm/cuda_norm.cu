#include "Timer.hpp"
#include <iostream>


__global__
void sq(int n, float *x, float *y){
     /* Write me -- kernel */
}


int main(int argc, char* argv[]) {

  size_t exponent           = 27;
  size_t num_trips          = 1;

  if (argc >= 2) exponent   = std::stol(argv[1]);
  if (argc >= 3) num_trips  = std::stol(argv[2]);

  size_t N = 1 << exponent;
  int block_size = 256;
  int num_blocks = (N + block_size - 1) / block_size;

  float *x = nullptr, *y = nullptr;

  cudaMallocManaged(&x, N*sizeof(float));
  cudaMallocManaged(&y, num_blocks*sizeof(float));

  for (int i = 0; i < N; i++) {
    x[i] = i;
  }

  DEF_TIMER(cuda_norm);
  START_TIMER(cuda_norm);
  cudaDeviceSynchronize();
  for (size_t i = 0; i < num_trips; ++i) {
    /* write me -- launch sq kernel */
    cudaDeviceSynchronize();
  }
  /* write me: final step, copy out values from y and add on cpu */

  double cuda_time = STOP_TIMER_QUIETLY(cuda_norm);
  std::cout << exponent << "\t" << num_trips << "\t" << cuda_time << std::endl;

  cudaFree(x); cudaFree(y);
  
  return 0;
}
