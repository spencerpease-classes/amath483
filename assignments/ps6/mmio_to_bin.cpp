







#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>




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





int main(int argc, char *argv[]) {

  auto inp = read_mm(argv[1]);
  
  std::ofstream ofp(argv[2], std::ios::binary);
  ofp << "%%MatrixMarket matrix coordinate pattern symmetric binary64" << std::endl;
  ofp << std::get<0>(inp) << " " << std::get<1>(inp) << " " << std::get<2>(inp).size() << std::endl;
  ofp.write((char*)std::get<2>(inp).data(), std::get<2>(inp).size()*sizeof(std::pair<size_t, size_t>));
  ofp.close();



  return 0;
}


#if 0
std::tuple<size_t, size_t, std::vector<std::tuple<size_t, size_t, double>>>
read_mm(std::string filename) {



  std::vector<std::pair<size_t, size_t>> aos(16), bos(16);

  std::fill(aos.begin(), aos.end(), std::make_pair<size_t, size_t>(1, -5));

  for (size_t i = 0; i < aos.size(); ++i) {
    aos[i].first = 1910987324109 * i - 23078923470*i *i -1; 
    aos[i].second = aos[i].first *1910987324109 * i - 23078923470*i *i;
  }


  std::ofstream o("aa.o", std::ios::binary);
  o << "this is a 1 2" << std::endl;
  o << "3 4" << std::endl;
  o.write((char*)aos.data(), aos.size()*sizeof(std::pair<size_t, size_t>));
  o.close();

  
  std::ifstream i("aa.o", std::ios::binary);
  std::string yack;
  std::getline(i, yack);
  std::getline(i, yack);
  i.read((char*)bos.data(), bos.size()*sizeof(std::pair<size_t, size_t>));
  
  auto is_equal = std::equal(aos.begin(), aos.end(), bos.begin(), 
			     [] (auto&a, auto&b) { return (a.first == b.first && a.second == b.second); } );

  if (is_equal) {
    std::cout << "yes!" << std::endl;
  } else {
    std::cout << "no... " << std::endl;
  }


  return 0;
}
#endif
