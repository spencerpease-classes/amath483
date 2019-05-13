//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH_583_BLUEBELT_AMATH583SPARSE_HPP
#define AMATH_583_BLUEBELT_AMATH583SPARSE_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

#include "COOMatrix.hpp"
#include "CSRMatrix.hpp"

#include "CSCMatrix.hpp"
#include "AOSMatrix.hpp"


// ----------------------------------------------------------------
//
// COOMatrix operations
//
// ----------------------------------------------------------------
void   piscetize(COOMatrix& A, size_t xpoints, size_t ypoints);

double one_norm(const COOMatrix& A);
double two_norm(const COOMatrix& A);
double inf_norm(const COOMatrix& A);
double Frobenius_norm(const COOMatrix& A);

Vector operator*(const COOMatrix& A, const Vector& x);
Matrix operator*(const COOMatrix& A, const Matrix& B);


// ----------------------------------------------------------------
//
// CSRMatrix operations
//
// ----------------------------------------------------------------
void   piscetize(CSRMatrix& A, size_t xpoints, size_t ypoints);

double one_norm(const CSRMatrix& A);
double two_norm(const CSRMatrix& A);
double inf_norm(const CSRMatrix& A);
double Frobenius_norm(const CSRMatrix& A);

Vector operator*(const CSRMatrix& A, const Vector& x);
Matrix operator*(const CSRMatrix& A, const Matrix& B);


// ----------------------------------------------------------------
//
// CSCMatrix operations
//
// ----------------------------------------------------------------
void   piscetize(CSCMatrix& A, size_t xpoints, size_t ypoints);
Vector operator*(const CSCMatrix& A, const Vector& x);
Matrix operator*(const CSCMatrix& A, const Matrix& x);


// ----------------------------------------------------------------
//
// AOSMatrix operations
//
// ----------------------------------------------------------------
void   piscetize(AOSMatrix& A, size_t xpoints, size_t ypoints);
Vector operator*(const AOSMatrix& A, const Vector& x);
Matrix operator*(const AOSMatrix& A, const Matrix& x);

#endif    //  AMATH_583_BLUEBELT_AMATH583SPARSE_HPP
