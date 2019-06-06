#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sequence.h>

#include "Timer.hpp"
#include <iostream>
#include <string>

template<typename T>
struct add {
  __host__ __device__ T operator()(const T& x, const T& y) const { return x + y + .15; }
};

int main(int argc, char* argv[]) {

  size_t exponent  = 24;
  size_t num_trips = 1;

  if (argc >= 2) exponent = std::stol(argv[1]);
  if (argc >= 3) num_trips = std::stol(argv[2]);

  size_t N = 1 << exponent;

  thrust::host_vector<float>   x(N), y(N);
  thrust::device_vector<float> X(N), Y(N);

  for (int i = 0; i < N; i++)
    y[i] = 2.0 * (x[i] = 1.0f);

  thrust::copy(x.begin(), x.end(), X.begin());
  thrust::copy(y.begin(), y.end(), Y.begin());

  DEF_TIMER(cuda_call);
  START_TIMER(cuda_call);
  for (size_t i = 0; i < num_trips; ++i) {
    cudaDeviceSynchronize();
    thrust::transform(X.begin(), X.end(), Y.begin(), Y.begin(), thrust::plus<float>());
  }
  cudaDeviceSynchronize();
  thrust::copy(Y.begin(), Y.end(), y.begin());

  double gpu_time = STOP_TIMER_QUIETLY(cuda_call);

  std::cout << exponent << "\t" << num_trips << "\t" << gpu_time << std::endl;

  return 0;
}
