//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_BLUEBELT_AMATH583IO_HPP
#define AMATH583_BLUEBELT_AMATH583IO_HPP


#include "COOMatrix.hpp"
#include "CSRMatrix.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <fstream>
#include <iostream>
#include <string>

#include "CSCMatrix.hpp"
#include "AOSMatrix.hpp"

Vector read_vector(const std::string& str);
void write_vector(const Vector& x, const std::string& str = "");

Matrix read_matrix(const std::string& str);
void write_matrix(const Matrix& A, const std::string& str = "");

COOMatrix read_coomatrix(const std::string& str);
void write_coomatrix(const COOMatrix& A, const std::string& str);
CSRMatrix read_csrmatrix(const std::string& str);
void write_csrmatrix(const CSRMatrix& A, const std::string& str);
CSCMatrix read_cscmatrix(const std::string& str);
void write_cscmatrix(const CSCMatrix& A, const std::string& str);
AOSMatrix read_aosmatrix(const std::string& str);
void write_aosmatrix(const AOSMatrix& A, const std::string& str);

#endif    // AMATH583_BLUEBELT_AMATH583IO_HPP
