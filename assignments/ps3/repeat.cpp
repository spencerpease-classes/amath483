//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#include <iostream>

int main(int argc, char *argv[]) {
  for (size_t i = 0; i < argc; ++i) {
    std::cout << "argv[" << i << "]: " <<argv[i] << std::endl;
  }

  return 0;
}
