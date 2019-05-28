//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <cmath>
#include <iostream>
#include "Timer.hpp"
#include "amath583.hpp"
#include "amath583IO.hpp"
#include "amath583sparse.hpp"
#include "Vector.hpp"
#include "omp.h"

#include "COOMatrix.hpp"
#include "CSRMatrix.hpp"

#ifdef TIME_CSC
#include "CSCMatrix.hpp"
#endif

#ifdef TIME_AOS
#include "AOSMatrix.hpp"
#endif

#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;



template <typename Matrix_t, typename Vector_t>
void sparse_matvec_bench(std::function<void (const Matrix_t&, const Vector_t&,Vector_t&)> f,
			 std::function<void (Matrix_t&, size_t)> partition,
			 std::vector<double>& sizes, std::vector<double>& flops, size_t maxthreads, const std::string& input_file) {

  std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
  sizes.clear();
  flops.clear();

  CSRMatrix A = read_csrmatrix(input_file);
  size_t size = A.num_rows();
  Vector_t x(size), y(size);
  randomize(x);

  for (size_t threads = 1; threads <= maxthreads; threads *= 2) {

    partition(A, threads);
    
    size_t trips = 4 + 64 * 1024 * 1024 / (2.0 * A.num_nonzeros());
    
    Timer t;
    t.start();
    for (size_t trip = 0; trip <= trips; ++trip) {
      f(std::cref(A), std::cref(x), std::ref(y));
    }
    t.stop();

    double f = (2.0 * A.num_nonzeros() ) / 1.E9;
    double e = ( t.elapsed()/((double) trips) ) / 1.E3;
    if (e == 0.0) e = 10000;

    flops.push_back(f / e );
    sizes.push_back(threads);

    std::cout << threads << "\t" << trips << "\t" << e << "\t" << e/trips << "\t" << f/e << "\t" << size << std::endl;

  }
}


void usage(const std::string& msg) {
  std::cout << "Usage: " + msg << " " << std::endl;
}


int main(int argc, char* argv[]) {

  size_t      maxthreads = 16;
  std::string input_file = "";

  try {
    for (int arg = 1; arg < argc; ++arg) {
      if (std::string(argv[arg]) == "-i") {
        if (argc == ++arg) usage(argv[0]);
        input_file = std::string(argv[arg]);
      } else if (std::string(argv[arg]) == "-t") {
        if (argc == ++arg) usage(argv[0]);
        maxthreads = std::stol(argv[arg]);
      } 
    }
  } catch (int) {
    usage(argv[0]);
    return -1;
  }

  std::vector<double> sizes(0);
  std::vector<double> flops(0);

  plt::figure_size(1280, 960);  // Initialize a 1280 X 960 figure


  std::cout << "================================================================" << std::endl;
  std::cout << "  Compressed sparse row matrix by row" << std::endl;

  sparse_matvec_bench<CSRMatrix, Vector>
    ([](const CSRMatrix& A, const Vector& x, Vector& y) { A.par_matvec(x,y); },
     [](CSRMatrix& A, size_t parts) { A.partition_by_row(parts); },
     sizes, flops, maxthreads, input_file);

  plt::named_loglog("CSR by row", sizes, flops);


  std::cout << "================================================================" << std::endl;
  std::cout << "  Compressed sparse row matrix by nnz" << std::endl;

  sparse_matvec_bench<CSRMatrix, Vector>
    ([](const CSRMatrix& A, const Vector& x, Vector& y) { A.par_matvec(x,y); },
     [](CSRMatrix& A, size_t parts) { A.partition_by_nnz(parts); },
     sizes, flops, maxthreads, input_file);

  plt::named_loglog("CSR by nnz", sizes, flops);


  plt::xlim(1, (int) maxthreads);
  plt::xlabel("Threads");
  plt::ylabel("GFlops");
  plt::title("Sparse Matrix Comparison of Partitioning " + input_file);
  plt::legend();
  plt::save("csr_scale.png");

  return 0;
}


