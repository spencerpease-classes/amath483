
#include "CSRMatrix.hpp"
#include "Timer.hpp"
#include "Vector.hpp"
#include "amath583.hpp"
#include "amath583IO.hpp"
#include "amath583sparse.hpp"
#include <cassert>
#include <cstddef>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include "omp.h"

Vector pagerank(const CSRMatrix& P, double alpha, double tol = 1.e-6, size_t max_iters = 8192) {

  Vector x(P.num_rows());
  randomize(x);
  x = (1.0 / one_norm(x)) * x;

  for (size_t i = 0; i < max_iters; ++i) {
    Vector y = alpha * (P * x) + (1.0 - alpha) / x.num_rows();

    if (two_norm(x - y) < tol) {
      std::cout << "Converged in " << i << " iterations" << std::endl;
      return y;
    }
    x = y;
  }
  return x;
}

auto rank(const Vector& x) {
  std::vector<std::pair<double, size_t>> rp(x.num_rows());

  for (size_t i = 0; i < x.num_rows(); ++i) {
    std::get<0>(rp[i]) = x(i);
    std::get<1>(rp[i]) = i;
  }
  std::sort(rp.begin(), rp.end(), [](auto& a, auto& b) -> bool { return (std::get<0>(a) > std::get<0>(b)); });

  std::vector<size_t> r(x.num_rows());

  for (size_t i = 0; i < x.num_rows(); ++i) {
    r[i] = std::get<1>(rp[i]);
  }

  return r;
}

std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> read_mm(std::istream& input_stream);
std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> read_mm(std::string filename);

auto read_stochastic_csrmatrix(const std::string& filename) {
  std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> pack = read_mm(filename);

  size_t                                          M   = std::get<0>(pack);
  size_t                                          N   = std::get<1>(pack);
  std::vector<std::tuple<size_t, size_t, double>> aos = std::get<2>(pack);

  Vector d(M);

  for (size_t i = 0; i < aos.size(); ++i) {
    d(std::get<1>(aos[i])) += 1.0;
  }

  // sort by row
  std::sort(aos.begin(), aos.end(), [](auto& a, auto& b) -> bool { return (std::get<0>(a) < std::get<0>(b)); });

  CSRMatrix A(M, N);
  A.open_for_push_back();

  for (size_t k = 0; k < aos.size(); ++k) {
    size_t i = std::get<0>(aos[k]);
    size_t j = std::get<1>(aos[k]);
    double v = std::get<2>(aos[k]) / d(j);
    A.push_back(i, j, v);
  }
  A.close_for_push_back();

  return A;
}

auto piscetize_stochastic_csrmatrix(size_t psize) {
  size_t    M = psize * psize;
  size_t    N = psize * psize;
  CSRMatrix A(M, N);
  A.open_for_push_back();

  for (size_t j = 0; j < psize; j++) {
    for (size_t k = 0; k < psize; k++) {
      size_t jrow = j * psize + k;

      if (j != 0) {
        size_t jcol = (j - 1) * psize + k;
        A.push_back(jrow, jcol, -1.0);
      }
      if (k != 0) {
        size_t jcol = j * psize + (k - 1);
        A.push_back(jrow, jcol, -1.0);
      }

      A.push_back(jrow, jrow, 4.0);

      if (k != psize - 1) {
        size_t jcol = j * psize + (k + 1);
        A.push_back(jrow, jcol, -1.0);
      }
      if (j != psize - 1) {
        size_t jcol = (j + 1) * psize + k;
        A.push_back(jrow, jcol, -1.0);
      }
    }
  }
  A.close_for_push_back();

  return A;
}

auto read_labels(const std::string& filename) {
  std::ifstream            input_file(filename);
  std::vector<std::string> labels;

  std::string string_input;
  while (std::getline(input_file, string_input)) {
    labels.push_back(string_input);
  }
  return labels;
}

void usage(const std::string& msg) {
  std::cout << "Usage: " + msg << " [-l label_file] [-o output_file] [-t num_threads] [-p piscetize size] [-v N] input_file" << std::endl;
}


int main(int argc, char* argv[]) {

  std::string input_file  = "";
  std::string out_file    = "";
  std::string label_file  = "";
  size_t      num_threads = 1;
  size_t      psize       = 0;
  int         verbose     = -1;

  try {
    for (int arg = 1; arg < argc; ++arg) {
      if (std::string(argv[arg]) == "-v") {
        if (argc == ++arg) usage(argv[0]);
        verbose = std::stol(argv[arg]);
      } else if (std::string(argv[arg]) == "-p") {
        if (argc == ++arg) usage(argv[0]);
        psize = std::stol(argv[arg]);
      } else if (std::string(argv[arg]) == "-o") {
        if (argc == ++arg) usage(argv[0]);
        out_file = std::string(argv[arg]);
      } else if (std::string(argv[arg]) == "-l") {
        if (argc == ++arg) usage(argv[0]);
        label_file = std::string(argv[arg]);
      } else if (std::string(argv[arg]) == "-t") {
        if (argc == ++arg) usage(argv[0]);
        num_threads = std::stol(argv[arg]);
      } else {
        if (input_file != "") {
          usage(argv[0]);
          return 1;
        }
        input_file = std::string(argv[arg]);
      }
    }
  } catch (int) {
    usage(argv[0]);
    return -1;
  }

  if (input_file == "" && psize == 0) {
    usage(argv[0]);
    return 1;
  }

  DEF_TIMER(read);
  START_TIMER(read);
  CSRMatrix A = (psize == 0 ? read_stochastic_csrmatrix(input_file) : piscetize_stochastic_csrmatrix(psize));
  STOP_TIMER(read);

  assert(A.num_rows() != 0);
  assert(A.num_cols() != 0);
  if (A.num_rows() != A.num_cols()) {
    std::cerr << "Error: Matrix " << input_file << " must be symmetric and is instead " << A.num_rows() << " by " << A.num_cols()
              << std::endl;
    return -3;
  }


  // ================================================================
  //
  // Call pagerank
  //
  // ================================================================
  DEF_TIMER(pagerank);
  START_TIMER(pagerank);
  Vector x = pagerank(A, 0.9);
  STOP_TIMER(pagerank);


  // ================================================================
  //
  // Rank order the result
  //
  // ================================================================
  DEF_TIMER(rank);
  START_TIMER(rank);
  auto r = rank(x);
  STOP_TIMER(rank);

  if (verbose == 0) {
    verbose = x.num_rows();
  }
    
  if (label_file != "") {
    std::vector<std::string> labels = read_labels(label_file);
    for (int i = 0; i < verbose; ++i) {
      std::cout << i;
      std::cout << "\t" << r[i];
      std::cout << "\t" << x(r[i]);
      std::cout << "\t" << labels[r[i]];
      std::cout << std::endl;
    }
  } else {
    for (int i = 0; i < verbose; ++i) {
      std::cout << i;
      std::cout << "\t" << r[i];
      std::cout << "\t" << x(r[i]);
      std::cout << std::endl;
    }
  }

  return 0;
}
