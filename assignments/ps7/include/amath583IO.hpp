//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_BROWNBELT_AMATH583IO_HPP
#define AMATH583_BROWNBELT_AMATH583IO_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>


// ----------------------------------------------------------------
//
// Vector I/O
//
// ----------------------------------------------------------------
template <typename VectorType>
VectorType read_vector(std::istream& input_stream) {
  std::string string_input;

  getline(input_stream, string_input);
  if (string_input.compare("AMATH 583 VECTOR") != 0) {
    std::cout << "Bad header " << string_input << std::endl;
    throw;
  }

  getline(input_stream, string_input);
  if (string_input.compare("THIS IS THE END") == 0) {
    return VectorType(0);
  }

  if (stol(string_input) < 0) {
    std::cout << "Error: vector file contains invalid vector length.";
    throw;
  }

  size_t N = stol(string_input);
  VectorType v(N);

  for (size_t i = 0; i < N; ++i) {
    getline(input_stream, string_input);
    v(i) = stod(string_input);
  }
  getline(input_stream, string_input);
  if (string_input.compare("THIS IS THE END") != 0) {
    std::cout << "Bad tailer " << string_input << std::endl;
    throw;
  }

  return v;
}

template <typename VectorType>
VectorType read_vector(const std::string& filename) {
  std::ifstream input_file(filename);
  return read_vector(input_file);
}


template <typename VectorType>
void stream_vector(const VectorType& x, std::ostream& outputFile) {
  outputFile << "AMATH 583 VECTOR" << std::endl;
  outputFile << x.num_rows() << std::endl;

  for (size_t i = 0; i < x.num_rows(); ++i) {    // Write data
    outputFile << std::setprecision(15) << std::scientific << x(i) << std::endl;
  }

  outputFile << "THIS IS THE END" << std::endl;    // Write tailer
}


template <typename VectorType>
void write_vector(const VectorType& x, const std::string& filename) {
  if (filename == "") {
    stream_vector(x, std::cout);
  } else {
    std::ofstream output_file(filename);
    stream_vector(x, output_file);
  }
}


// ----------------------------------------------------------------
//
// Matrix I/O
//
// ----------------------------------------------------------------
template <typename MatrixType>
MatrixType read_matrix(std::istream& input_stream) {
  std::string string_input;

  getline(input_stream, string_input);
  if (string_input.compare("AMATH 583 MATRIX") != 0) {
    std::cout << "Error: matrix file does not contain a header.";
    throw;
  }

  getline(input_stream, string_input);
  if (string_input.compare("THIS IS THE END") == 0) return MatrixType(0, 0);

  long num_rows = stol(string_input);
  if (num_rows < 0) {
    std::cout << "Error: matrix file contains invalid num rows";
    throw;
  }

  getline(input_stream, string_input);
  long num_cols = stol(string_input);
  if (num_cols < 0) {
    std::cout << "Error: matrix file contains invalid num columns";
    throw;
  }

  MatrixType A(num_rows, num_cols);

  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      getline(input_stream, string_input);
      A(i, j) = stod(string_input);
    }
  }
  getline(input_stream, string_input);
  if (string_input.compare("THIS IS THE END") != 0) {
    std::cout << "Error: vector file does not contain proper footer.";
    throw;
  }
  return A;
}


template <typename MatrixType>
MatrixType read_matrix(const std::string& filename) {
  std::ifstream input_file(filename);
  return read_matrix(input_file);
}


template <typename MatrixType>
void stream_matrix(const MatrixType& A, std::ostream& outputFile) {
  outputFile << "AMATH 583 MATRIX" << std::endl;    // Write header
  outputFile << A.num_rows() << std::endl;
  outputFile << A.num_cols() << std::endl;

  for (size_t i = 0; i < A.num_rows(); i++) {
    for (size_t j = 0; j < A.num_cols(); j++) {
      outputFile << std::setprecision(15) << std::scientific << A(i, j) << std::endl;
    }
  }

  outputFile << "THIS IS THE END" << std::endl;    // Write tailer
}


template <typename MatrixType>
void write_matrix(const MatrixType& A, const std::string& filename) {
  if (filename == "") {
    stream_matrix(A, std::cout);
  } else {
    std::ofstream outputFile(filename);
    stream_matrix(A, outputFile);
  }
}


// ----------------------------------------------------------------
//
// Sparse I/O
//
// ----------------------------------------------------------------
std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>>
read_mm(std::istream& input_stream) {
  std::string              string_input;
  bool                     file_symmetry = false;
  bool                     pattern = false;
  bool                     binary64 = false;
  std::vector<std::string> header(6);

  // %%MatrixMarket matrix coordinate integer symmetric
  // %%MatrixMarket matrix coordinate real general [binary32, binary64]
  // %%MatrixMarket matrix coordinate pattern symmetric

  std::getline(input_stream, string_input);
  std::stringstream h(string_input);
  for (auto& s : header)
    h >> s;

  if (   header[0] != "%%MatrixMarket"
      || header[1] != "matrix"
      || header[2] != "coordinate") {

    std::cerr << "Unsupported format: " << 
      (header[0] != "%%MatrixMarket") << " " <<
      (header[1] != "matrix") << " " <<
      (header[2] != "coordinate") << std::endl;


    std::cerr << "Unsupported format: " << header[0] << " " << header[1] << " " << header[2] << std::endl;
    throw;
  }

  if (header[3] == "pattern") {
    pattern = true;
  } else if (header[3] == "real" || header[3] == "integer") {
    pattern = false;
  } else {
    std::cerr << "Bad mmio format (value type): " << header[3] << std::endl;
    throw;
  }

  if (header[4] == "symmetric") {
    file_symmetry = true;
  } else if (header[4] == "unsymmetric" || header[4] == "general") {
    file_symmetry = false;
  } else {
    std::cerr << "Bad mmio format (symmetry): " << header[4] << std::endl;
    throw;
  }

  if (header[5] != "") {
    if (header[5] == "binary64") {
      binary64 = true;
    } else {
      throw;
    }
  } 

  while (std::getline(input_stream, string_input)) {
    if (string_input[0] != '%') break;
  }
  size_t n0, n1, nnz;
  std::stringstream(string_input) >> n0 >> n1 >> nnz;

  std::vector<std::tuple<size_t, size_t, double>> aos(0);
  aos.reserve(nnz);


  if (binary64) {
    std::vector<std::tuple<size_t, size_t>> bos(nnz);
    input_stream.read((char*)bos.data(), bos.size()*sizeof(std::tuple<size_t, size_t>));
    for (auto &j : bos) {
      aos.push_back( { std::get<0>(j), std::get<1>(j), 1.0 } );
    }


  } else {

    for (size_t i = 0; i < nnz; ++i) {
      std::string buffer;
      size_t      d0, d1;
      
      std::getline(input_stream, buffer);
      
      double      v = 1.0;
      if (true == pattern) {
	std::stringstream(buffer) >> d0 >> d1;
      } else {
	std::stringstream(buffer) >> d0 >> d1 >> v;
      }
      aos.push_back({ d0 - 1, d1 - 1, 1.0 });
      if (file_symmetry == true) {
	aos.push_back({ d1 - 1, d0 - 1, 1.0 });
      }
    }
  }

  return std::make_tuple(n0, n1, aos);
}


std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>>
read_mm(std::string filename) {
  std::ifstream input_file(filename, std::ios::binary);
  return read_mm(input_file);
}


// template <typename MatrixType>
// MatrixType read_matrix(const std::string& filename) {

template <>
COOMatrix read_matrix(const std::string& filename) {
  std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> pack = read_mm(filename);

  size_t M = std::get<0>(pack);
  size_t N = std::get<1>(pack);
  std::vector<std::tuple<size_t, size_t, double>> aos = std::get<2>(pack);

  COOMatrix A(M, N);
  A.reserve(aos.size());

  for (size_t k = 0; k < aos.size(); ++k) {
    size_t i = std::get<0>(aos[k]);
    size_t j = std::get<1>(aos[k]);
    double v = std::get<2>(aos[k]);
    A.push_back(i, j, v);
  }

  return A;
}


template <>
COOMatrix write_matrix(const COOMatrix& A, const std::string& filename) {
  std::ofstream output_file(filename);
  output_file << "%%MatrixMarket matrix coordinate real general" << std::endl;
  output_file << A.num_rows() << " " << A.num_cols() << " " << A.num_nonzeros() << std::endl;
  A.stream_coordinates(output_file);
}


template <>
CSRMatrix read_csrmatrix(const std::string& filename) {
  std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> pack = read_mm(filename);

  size_t M = std::get<0>(pack);
  size_t N = std::get<1>(pack);
  std::vector<std::tuple<size_t, size_t, double>> aos = std::get<2>(pack);

  // sort by row
  std::sort(aos.begin(), aos.end(), [] (auto &a, auto &b) -> bool {
      return (std::get<0>(a) < std::get<0>(b) );
    } );
  
  CSRMatrix A(M, N);
  A.open_for_push_back();

  for (size_t k = 0; k < aos.size(); ++k) {
    size_t i = std::get<0>(aos[k]);
    size_t j = std::get<1>(aos[k]);
    double v = std::get<2>(aos[k]);
    A.push_back(i, j, v);
  }
  A.close_for_push_back();

  return A;
}


template <>
void write_matrix(const CSRMatrix& A, std::string filename) {
  std::ofstream output_file(filename);
  std::cout << "%%MatrixMarket matrix coordinate real general" << std::endl;
  std::cout << A.num_rows() << " " << A.num_cols() << " " << A.num_nonzeros() << std::endl;
  A.stream_coordinates(output_file);
}


template <>
CSCMatrix read_matrix(const std::string& filename) {
  std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> pack = read_mm(filename);

  size_t M = std::get<0>(pack);
  size_t N = std::get<1>(pack);
  std::vector<std::tuple<size_t, size_t, double>> aos = std::get<2>(pack);

  // sort by column
  std::sort(aos.begin(), aos.end(), [] (auto &a, auto &b) -> bool {
      return (std::get<1>(a) < std::get<1>(b) );
    } );
  
  CSCMatrix A(M, N);
  A.open_for_push_back();

  for (size_t k = 0; k < aos.size(); ++k) {
    size_t i = std::get<0>(aos[k]);
    size_t j = std::get<1>(aos[k]);
    double v = std::get<2>(aos[k]);
    A.push_back(i, j, v);
  }
  A.close_for_push_back();

  return A;
}  

template <>
void write_matrix(const CSCMatrix& A, std::string filename) {
  std::ofstream output_file(filename);
  std::cout << "%%MatrixMarket matrix coordinate real general" << std::endl;
  std::cout << A.num_rows() << " " << A.num_cols() << " " << A.num_nonzeros() << std::endl;
  A.stream_coordinates(output_file);
}


template <>
AOSMatrix read_matrix(const std::string& filename) {
  std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>> pack = read_mm(filename);

  size_t M = std::get<0>(pack);
  size_t N = std::get<1>(pack);
  std::vector<std::tuple<size_t, size_t, double>> aos = std::get<2>(pack);

  AOSMatrix A(M, N);
  A.reserve(aos.size());

  for (size_t k = 0; k < aos.size(); ++k) {
    size_t i = std::get<0>(aos[k]);
    size_t j = std::get<1>(aos[k]);
    double v = std::get<2>(aos[k]);
    A.push_back(i, j, v);
  }

  return A;
}


template <>
void write_matrix(const AOSMatrix& A, std::string filename) {
  std::ofstream output_file(filename);
  std::cout << "%%MatrixMarket matrix coordinate real general" << std::endl;
  std::cout << A.num_rows() << " " << A.num_cols() << " " << A.num_nonzeros() << std::endl;
  A.stream_coordinates(output_file);
}


auto
read_mat(std::istream& input_stream) {

  // array 
}

#endif // AMATH583_BROWNBELT_AMATH583IO_HPP
