#define CATCH_CONFIG_MAIN    // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <cstddef>
#include "Matrix.hpp"
#include "amath583.hpp"

TEST_CASE("Matrix operations", "[Zero]") {
  size_t N = 256;
  Matrix A(N, N), B(N, N), C(N, N), D(N, N);

  REQUIRE(A.num_rows() == N);
  REQUIRE(B.num_rows() == N);
  REQUIRE(C.num_rows() == N);
  REQUIRE(D.num_rows() == N);

  REQUIRE(A.num_cols() == N);
  REQUIRE(B.num_cols() == N);
  REQUIRE(C.num_cols() == N);
  REQUIRE(D.num_cols() == N);

  REQUIRE(Frobenius_norm(A) == 0.0);
  REQUIRE(Frobenius_norm(B) == 0.0);
  REQUIRE(Frobenius_norm(C) == 0.0);
  REQUIRE(Frobenius_norm(D) == 0.0);


  SECTION("test randomize") {  
    randomize(A); 
    REQUIRE(Frobenius_norm(A) != 0.0);
    
    randomize(B); 
    REQUIRE(Frobenius_norm(B) != 0.0);
    
    randomize(C); 
    REQUIRE(Frobenius_norm(C) != 0.0);
    
    randomize(D);
    REQUIRE(Frobenius_norm(D) != 0.0);
  }
  SECTION("test zeroize") {  
    zeroize(A);
    REQUIRE(Frobenius_norm(A) == 0.0);
    
    zeroize(B); 
    REQUIRE(Frobenius_norm(B) == 0.0);
    
    zeroize(C); 
    REQUIRE(Frobenius_norm(C) == 0.0);
    
    zeroize(D);
    REQUIRE(Frobenius_norm(D) == 0.0);
  }    
}

TEST_CASE("Matrix loop order", "[Order]") {
  size_t N = 256;
  Matrix A(N, N), B(N, N), C(N, N), D(N, N);


  SECTION("test ijk (kind of)") {  
    Matrix E = A * B;
    zeroize(C);
    matmat(A, B, C);
    REQUIRE(Frobenius_norm(E-C) == 0);
  }

  zeroize(D);
  matmat(A, B, D);

  SECTION("test ikj order") {
    zeroize(C);
    matmat_ikj(A, B, C);
    REQUIRE(Frobenius_norm(D-C) < 1.e-12);    
  }

  SECTION("test jki order") {
    zeroize(C);
    matmat_jki(A, B, C);
    REQUIRE(Frobenius_norm(D-C) < 1.e-12);    
  }
}  
