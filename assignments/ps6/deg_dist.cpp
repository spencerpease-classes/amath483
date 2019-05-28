



#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <numeric>
#include <algorithm>


auto
read_mm(std::istream& input_stream) {
  std::string              string_input;
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

  std::vector<std::tuple<size_t, size_t>> aos(0);
  aos.reserve(nnz);


  if (binary64) {
    aos.resize(nnz);
    input_stream.read((char*)aos.data(), aos.size()*sizeof(std::tuple<size_t, size_t>));


  } else {

    for (size_t i = 0; i < nnz; ++i) {
      std::string buffer;
      size_t      d0, d1;
      double      v;
      
      std::getline(input_stream, buffer);
      
      if (true == pattern) {
	std::stringstream(buffer) >> d0 >> d1;
	aos.push_back({ d0 - 1, d1 - 1});
	// aos[i] = { d0 - 1, d1 - 1, 1.0 };
      } else {
	std::stringstream(buffer) >> d0 >> d1 >> v;
	aos.push_back({ d0 - 1, d1 - 1 });
	//aos[i] = { d0 - 1, d1 - 1, v };
      }
    }
  }

  return std::make_tuple(n0, n1, aos);
}

auto
read_mm(std::string filename) {
  std::ifstream input_file(filename, std::ios::binary);
  return read_mm(input_file);
}


#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


int main(int argc, char *argv[]) {

  auto inp = read_mm(argv[1]);
  
  std::vector<size_t> degrees(std::get<0>(inp));
  std::vector<size_t> row_nums(std::get<0>(inp));
  std::iota(row_nums.begin(), row_nums.end(), 1);

  for (auto& x : std::get<2>(inp)) {
    ++degrees[std::get<0>(x)];
  }
  std::sort(degrees.begin(), degrees.end(), std::greater<size_t>());
  
  plt::figure_size(1280, 960);  // Initialize a 1280 X 960 figure
  plt::xlim(1, (int) std::get<0>(inp));

  plt::named_loglog("degree", row_nums, degrees);

  plt::xlabel("row number");
  plt::ylabel("degree");
  plt::title("Degree distribution");

  plt::legend();

  plt::save("deg_dist.png");



  return 0;
}
