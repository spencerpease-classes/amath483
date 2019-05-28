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
#include "Vector.hpp"
#include "amath583.hpp"

#include "omp.h"


#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


void norm_bench(std::function<double (const Vector&)>&& f,
		std::vector<double>& sizes, std::vector<double>& flops, size_t size, size_t maxthreads, const std::string& scaling) {
  std::cout << "N\ttrips\ttotal\tavg\tGflops" << std::endl;
  sizes.clear();
  flops.clear();

  for (size_t threads = 1; threads <= maxthreads; threads *= 2) {

    omp_set_num_threads(threads);

    Vector x(size);
    randomize(x);
    double ref_norm = two_norm(x);
    double check_norm = f(x);

    if (std::abs(ref_norm - check_norm)/ref_norm > 1.e-12 * x.num_rows()) {
      std::cout << "Error too large: " << std::abs(ref_norm - check_norm)/ref_norm << std::endl;
    }
    
    size_t trips = 4 + 1024 * 1024 * 1024 / (2.0 * x.num_rows());
    
    Timer t;
    t.start();
    for (size_t trip = 0; trip <= trips; ++trip) {
      check_norm += f(std::cref(x));
    }
    t.stop();

    double f = (2.0 * x.num_rows() ) / 1.E9;
    double e = ( t.elapsed()/((double) trips) ) / 1.E3;
    if (e == 0.0) e = 10000;

    flops.push_back(f / e );
    sizes.push_back(threads);

    std::cout << threads << "\t" << trips << "\t" << e << "\t" << e/trips << "\t" << f/e << "\t" << size << "\t" << check_norm << std::endl;

    if (scaling == "weak") {
      size *= 2;
    }
  }
}


double norm_seq(const Vector& x) {
  double sum = 0;

  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);  
}


double norm_parfor(const Vector& x) {
  double sum = 0;
#pragma omp parallel for reduction(+:sum)
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double norm_block_reduction(const Vector& x) {
  double sum = 0;
  /* Fix Me */
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double norm_block_critical(const Vector& x) {
  double sum = 0;
  /* Fix Me */
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double norm_cyclic_reduction(const Vector& x) {
  double sum = 0;
  /* Fix Me */
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double norm_cyclic_critical(const Vector& x) {
  double sum = 0;
  /* Fix Me */
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}


void usage(const std::string& msg) {
  std::cout << "Usage: " + msg << " [-s strong/weak] [-t maxthreads] [-N size]" << std::endl;
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
	  usage(argv[0]);
	  throw;
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

  std::cout << "================================================================" << std::endl;
  std::cout << "  Sequential " << std::endl;

  norm_bench (norm_seq, sizes, flops, size, 1, scaling);
  sizes.push_back(maxthreads);
  flops.push_back(flops[0]);
  plt::named_loglog("Sequential", sizes, flops);

  std::cout << "================================================================" << std::endl;
  std::cout << "  Parallel For " << std::endl;

  norm_bench (norm_parfor, sizes, flops, size, maxthreads, scaling);
  plt::named_loglog("Parallel For", sizes, flops);

  std::cout << "================================================================" << std::endl;
  std::cout << "  Block reduction " << std::endl;

  norm_bench (norm_block_reduction, sizes, flops, size, maxthreads, scaling);
  plt::named_loglog("Block reduction", sizes, flops);

  std::cout << "================================================================" << std::endl;
  std::cout << "  Block critical " << std::endl;

  norm_bench (norm_block_critical, sizes, flops, size, maxthreads, scaling);
  plt::named_loglog("Block critical", sizes, flops);

  std::cout << "================================================================" << std::endl;
  std::cout << "  Cyclic reduction " << std::endl;
  
  norm_bench (norm_cyclic_reduction, sizes, flops, size, maxthreads, scaling);
  plt::named_loglog("Cyclic reduction", sizes, flops);

  std::cout << "================================================================" << std::endl;
  std::cout << "  Cyclic critical " << std::endl;

  norm_bench (norm_cyclic_critical, sizes, flops, size, maxthreads, scaling);
  plt::named_loglog("Cyclic critical", sizes, flops);

  plt::xlim(1, (int) maxthreads);
  plt::xlabel("Threads");
  plt::ylabel("GFlops");
  plt::title("Two Norm Scaling OpenMP " + scaling + " scaling, size =" + std::to_string(size));

  plt::legend();

  plt::save("norm_scale.png");

  return 0;
}


