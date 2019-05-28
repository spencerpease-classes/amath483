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

#ifdef TIME_COO
#include "COOMatrix.hpp"
#endif

#ifdef TIME_CSR
#include "CSRMatrix.hpp"
#endif

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
			 std::vector<double>& sizes, std::vector<double>& flops, size_t size, size_t maxthreads, const std::string& scaling) {
  std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
  sizes.clear();
  flops.clear();

  for (size_t threads = 1; threads <= maxthreads; threads *= 2) {

    omp_set_num_threads(threads);

    Vector_t x(size), y(size), z(size);
    Matrix_t A(size, size);
    piscetize(A, std::sqrt(size), std::sqrt(size));
    randomize(x);
    A.matvec(x, z);
    f(std::cref(A), std::cref(x), std::ref(y));
    double nrm = two_norm(z-y);
    double res = nrm / two_norm(z);

    if (res > 1.e-12 * x.num_rows()) {
      std::cout << "Error too large: " << res << std::endl;
    }
    
    size_t trips = 4 + 1024 * 1024 * 1024 / (2.0 * A.num_nonzeros());
    
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

    if (scaling == "weak") {
      size *= 2;
    }
  }
}


void usage(const std::string& msg) {
  std::cout << "Usage: " + msg << " " << std::endl;
}


int main(int argc, char* argv[]) {

  size_t      maxthreads = 16;
  size_t      size       = 8192;
  std::string scaling = "strong";

  try {
    for (int arg = 1; arg < argc; ++arg) {
      if (std::string(argv[arg]) == "-s") {
        if (argc == ++arg) usage(argv[0]);
	scaling = std::string(argv[arg]);
	if (scaling != "strong" && scaling != "weak") {
	  throw 1;
	}
      } else if (std::string(argv[arg]) == "-N") {
        if (argc == ++arg) usage(argv[0]);
        size = std::stol(argv[arg]);
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

#ifdef TIME_COO
  std::cout << "================================================================" << std::endl;
  std::cout << "  Coordinate sparse matrix " << std::endl;

  sparse_matvec_bench<COOMatrix, Vector>
    ([](const COOMatrix& A, const Vector& x, Vector& y) { A.omp_matvec(x,y); },
     sizes, flops, size, maxthreads, scaling);

  plt::named_loglog("COO", sizes, flops);
#endif


#ifdef TIME_CSR
  std::cout << "================================================================" << std::endl;
  std::cout << "  Compressed sparse row matrix " << std::endl;

  sparse_matvec_bench<CSRMatrix, Vector>
    ([](const CSRMatrix& A, const Vector& x, Vector& y) { A.omp_matvec(x,y); },
     sizes, flops, size, maxthreads, scaling);

  plt::named_loglog("CSR", sizes, flops);
#endif


#ifdef TIME_CSC
  std::cout << "================================================================" << std::endl;
  std::cout << "  Compressed sparse column matrix " << std::endl;

  sparse_matvec_bench<CSCMatrix, Vector>
    ([](const CSCMatrix& A, const Vector& x, Vector& y) { A.omp_matvec(x,y); },
     sizes, flops, size, maxthreads, scaling);

  plt::named_loglog("CSC", sizes, flops);
#endif


#ifdef TIME_AOS
  std::cout << "================================================================" << std::endl;
  std::cout << "  Array of structs matrix " << std::endl;

  sparse_matvec_bench<AOSMatrix, Vector>
    ([](const AOSMatrix& A, const Vector& x, Vector& y) { A.omp_matvec(x,y); },
     sizes, flops, size, maxthreads, scaling);

  plt::named_loglog("AOS", sizes, flops);
#endif

  plt::xlim(1, (int) maxthreads);
  plt::xlabel("Threads");
  plt::ylabel("GFlops");
  plt::title("Sparse Matrix Scaling OpenMP " + scaling + " scaling, size =" + std::to_string(size));

  plt::legend();

  plt::save("matvec_scale.png");

  return 0;
}


