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
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __USE_CIMG
#include "CImg.h"
#endif

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Timer.hpp"

#include "amath583.hpp"

double sigma(double z) { return (1.0 / (1.0 + std::exp(-z))); }

double sigma_prime(double z) { return (sigma(z) * (1.0 - sigma(z))); }

Vector sigma(const Vector& z) {
  Vector x(z.num_rows());
  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = sigma(z(i));
  }
  return x;
}

Vector sigma_prime(const Vector& z) {
  Vector x(z.num_rows());
  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = sigma_prime(z(i));
  }
  return x;
}

Vector operator+(double b, const Vector& x) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = b + x(i);
  }
  return y;
}

Vector operator+(const Vector& x, double b) { return b + x; }

Vector scale(const Vector& x, const Vector& y) {
  assert(x.num_rows() == y.num_rows());
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) * y(i);
  }
  return z;
}

double read_double(const std::string& filename) {
  std::ifstream input_file(filename);

  double num;
  input_file >> num;
  return num;
}

Matrix read_matrix(const std::string& filename) {
  std::ifstream input_file(filename);
  std::string   foo;

  size_t rows, cols;
  input_file >> rows;
  input_file >> cols;
  getline(input_file, foo);

  Matrix A(rows, cols);

  for (size_t i = 0; i < rows; ++i) {
    std::string line;
    getline(input_file, line);
    std::stringstream stream_line(line);
    for (size_t j = 0; j < cols; ++j) {
      stream_line >> A(i, j);
    }
  }
  return A;
}

auto read_csv(const std::string& input) {
  auto ifs = std::ifstream(input);

  std::vector<Vector> ys;

  std::string line;

  while (std::getline(ifs, line)) {
    std::istringstream s(line);
    std::string        field;
    Vector             x(400);
    size_t             i = 0;
    while (getline(s, field, ',')) {
      x(i++) = stod(field);
    }
    ys.push_back(x);
  }
  return ys;
}

auto read_labels(const std::string& input) {
  auto ifs = std::ifstream(input);

  std::vector<size_t> labels;

  while (!ifs.eof()) {
    double d = 0;
    ifs >> d;
    labels.push_back(d);
  }

  return labels;
}

Vector trans_mult(const Matrix& A, const Vector& x) {
  assert(x.num_rows() == A.num_rows());
  Vector y(A.num_cols());
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(j) += A(i, j) * x(i);
    }
  }
  return y;
}

void rank_one(Matrix& A, double alpha, const Vector& u, const Vector& v) {
  // std::cout << "norm u, v = " << two_norm(u) << " " << two_norm(v) << std::endl;

  assert(A.num_rows() == u.num_rows());
  assert(A.num_cols() == v.num_rows());

  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) += alpha * u(i) * v(j);
    }
  }
}

#ifdef __USE_CIMG
// User interface support code

using namespace cimg_library;

const unsigned char black[] = {0, 0, 0};
const unsigned char white[] = {255, 255, 255};
const unsigned char gray[]  = {196, 196, 196};
const unsigned char red[]   = {255, 0, 0};

bool is_in_canvas(int x, int y) { return (x >= 0 && x < 20 && y >= 0 && y < 20); }

int is_in_button(int x, int y) {
  if (y >= 20 && y < 22 && x >= 0 && x < 20) {
    return x / 2;
  }
  return -1;
}

bool is_in_okay(int x, int y) { return (x >= 4 && x < 7 && y >= 22 && y < 24); }

bool is_in_clear(int x, int y) { return (x >= 13 && x < 17 && y >= 22 && y < 24); }

void disp_button(CImg<unsigned char>& image, int digit, const unsigned char fore[], const unsigned char back[]) {
  char buffer[1024];
  sprintf(buffer, "%d", digit);
  image.draw_text(digit * 20 + 3, 203, buffer, fore, back);
}
#endif

size_t random_index(size_t limit) {
  std::default_random_engine             generator;
  std::uniform_real_distribution<double> distribution(0, limit);
  static auto                            dice = std::bind(distribution, generator);

  return dice();
}

void usage(const std::string& msg) { std::cout << "Usage: " << msg << std::endl; }


void train_cpu(Matrix& W0, Matrix& W1, Vector& b1, Vector& b2, double alpha, size_t maxiter) {
  size_t output_nodes = W1.num_rows();

  randomize(W0, 0.06);
  randomize(W1, 0.06);
  randomize(b1, 0.06);
  randomize(b2, 0.06);
  
  std::vector<Vector> ys     = read_csv("data.csv");
  std::vector<size_t> labels = read_labels("dataLabels.csv");
  Vector              z(output_nodes);
  
  double rnorm = 0.0;
  double rnorm0 = 1.0;
  
  for (size_t iter = 0; iter < maxiter; ++iter) {
    rnorm = 0.0;
    
    for (size_t i = 0; i < ys.size(); ++i) {
      
      auto x0 = ys[i];
      
      // Forward
      auto y0 = W0 * x0 + b1;
      auto x1 = sigma(y0);
      auto y1 = W1 * x1 + b2;
      auto x2 = sigma(y1);
      
      // Back propagation
      zeroize(z);
      z(labels[i]) = 1.0;
      
      auto e2 = z - x2;
      
      rnorm += two_norm(e2);
      std::cout << two_norm(e2) << std::endl;
      
      auto d2 = scale(e2, sigma_prime(y1));
      auto d1 = scale(trans_mult(W1, d2), sigma_prime(y0));
      
      rank_one(W0, alpha, d1, x0);
      rank_one(W1, alpha, d2, x1);
      b1 = b1 + alpha * d1;
      b2 = b2 + alpha * d2;
    }
    
    if (iter == 0) {
      rnorm0 = rnorm;
    }
    std::cout << "|| x - z || = " << rnorm / rnorm0 << " " << rnorm << std::endl;
    if (rnorm / rnorm0 < 1.e-6) break;
  }
}


int main(int argc, char* argv[]) {

  double      alpha        = 0.1;
  size_t      maxiter      = 1;

  int         input_nodes  = 400;
  int         hidden_nodes = 15;
  int         output_nodes = 10;

  bool train = true;
  bool gpu   = false;

  try {
    for (int arg = 1; arg < argc; ++arg) {
      if (std::string(argv[arg]) == "-a") {
        if (argc == ++arg) usage(argv[0]);
        alpha = std::stod(argv[arg]);
      } else if (std::string(argv[arg]) == "-m") {
        if (argc == ++arg) usage(argv[0]);
        maxiter = std::stol(argv[arg]);
      } else if (std::string(argv[arg]) == "-h") {
        if (argc == ++arg) usage(argv[0]);
        hidden_nodes = std::stoi(argv[arg]);
      } else if (std::string(argv[arg]) == "-t") {
        train = false;
      } else if (std::string(argv[arg]) == "-g") {
        gpu = true;
      }
    }
  } catch (int) {
    usage(argv[0]);
    return -1;
  }

  Matrix W0 = train ? Matrix(hidden_nodes, input_nodes) : read_matrix("w0.txt");
  Matrix W1 = train ? Matrix(output_nodes, hidden_nodes) : read_matrix("w1.txt");

  if (!train) {
    hidden_nodes = W0.num_rows();
    output_nodes = W1.num_rows();
  }

  Vector b1(hidden_nodes);
  Vector b2(output_nodes);

  if (!train) {
    std::fill(&b1(0), &b1(0)+b1.num_rows(), read_double("b1.txt"));
    std::fill(&b2(0), &b2(0)+b2.num_rows(), read_double("b2.txt"));
  }

  if (train) {
    if (gpu) {
      DEF_TIMER(gpu);
      START_TIMER(gpu);
      // train_gpu(W0, W1, b1, b2, alpha, maxiter);
      STOP_TIMER(gpu);
    } else {
      DEF_TIMER(cpu);
      START_TIMER(cpu);
      train_cpu(W0, W1, b1, b2, alpha, maxiter);
      STOP_TIMER(cpu);
    }
  }


#ifdef __USE_CIMG

  // Initialize user interface
  CImg<unsigned char> image(200, 240, 1, 3, 255);
  CImgDisplay         main_disp(image, "Draw a digit");

  image.draw_rectangle(0, 200, 200, 240, gray);
  for (int i = 0; i < 10; ++i) {
    disp_button(image, i, black, gray);
  }
  image.draw_rectangle(0, 220, 200, 240, gray);
  image.draw_text(45, 223, "OK", black, gray);
  image.draw_text(130, 223, "Clear", black, gray);

  image.display(main_disp);

  Vector x0(input_nodes);

  // User interface processing loop
  while (!main_disp.is_closed()) {
    main_disp.wait();
    if (main_disp.button()) {
      const int x = main_disp.mouse_x() / 10;
      const int y = main_disp.mouse_y() / 10;

      if (is_in_canvas(x, y)) {    // User drew something (clicked pixel)
        image.draw_rectangle(10 * x, 10 * y, 10 * (x + 1), 10 * (y + 1), black);
        x0(x * 20 + y) = 1.0;           // Save pixel location to input vector
      } else if (is_in_okay(x, y)) {    // User clicked Okay

        // ================================================================
        // Forward propagation code
        auto x1 = sigma(W0 * x0 + b1);
        auto x2 = sigma(W1 * x1 + b2);
        // ================================================================

        // Locate largest element -- that's the digit
        auto   loc   = std::max_element(&x2(0), &x2(0) + x2.num_rows());
        size_t digit = loc - &x2(0);

        disp_button(image, digit, red, white);
      } else if (is_in_clear(x, y)) {                   // User clicked Clear
        image.draw_rectangle(0, 0, 200, 200, white);    // Clear canvas
        for (int i = 0; i < 10; ++i) {
          disp_button(image, i, black, gray);    // Redraw digits
        }
        zeroize(x0);    // Zero out input vector
      }
      image.display(main_disp);
    }
  }

#endif

  return 0;
}
