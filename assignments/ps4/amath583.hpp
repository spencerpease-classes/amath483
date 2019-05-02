//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH_583_GREENBELT_AMATH583_HPP
#define AMATH_583_GREENBELT_AMATH583_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

// ----------------------------------------------------------------
//
// Vector arithmetic
//
// ----------------------------------------------------------------
void   zeroize(Vector& x);
void   randomize(Vector& x);
double one_norm(const Vector& x);
double two_norm(const Vector& x);
double inf_norm(const Vector& x);
double p_norm(const Vector& x);

Vector operator+(const Vector& x, const Vector& y);
Vector operator-(const Vector& x, const Vector& y);

Vector& operator*=(double alpha, const Vector& x);
Vector& operator+=(const Vector& x, const Vector& y);
Vector& operator-=(const Vector& x, const Vector& y);

// ----------------------------------------------------------------
//
// Matrix arithmetic
//
// ----------------------------------------------------------------
void   zeroize(Matrix& A);
void   randomize(Matrix& A);
double one_norm(const Matrix& A);
double two_norm(const Matrix& A);
double inf_norm(const Matrix& A);
double Frobenius_norm(const Matrix& A);

Matrix operator+(const Matrix& x, const Matrix& y);
Matrix operator-(const Matrix& x, const Matrix& y);

Matrix& operator*=(double alpha, const Matrix& x);
Matrix& operator+=(const Matrix& x, const Matrix& y);
Matrix& operator-=(const Matrix& x, const Matrix& y);

// ----------------------------------------------------------------
//
// Matrix-vector operations
//
// ----------------------------------------------------------------
Vector operator*(const Matrix& A, const Vector& x);
void   matvec(const Matrix& A, const Vector& x, Vector& y);
/* Declaration of matvec_ji goes here (583) */
/* Declaration of matvec_ji goes here (583) */


// ----------------------------------------------------------------
//
// Matrix-matrix operations
//
// ----------------------------------------------------------------
Matrix operator*(const Matrix& A, const Matrix& B);
void   matmat(const Matrix& A, const Matrix& B, Matrix& C);
void   matmat_ikj(const Matrix& A, const Matrix& B, Matrix& C);
void   matmat_jki(const Matrix& A, const Matrix& B, Matrix& C);
/* Declaration of matmat_ikj goes here */
/* Declaration of matmat_jki goes here */


#endif    //  AMATH_583_GREENBELT_AMATH583_HPP
