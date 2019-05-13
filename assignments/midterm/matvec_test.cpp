//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#define CATCH_CONFIG_MAIN    // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <cstddef>
#include "Vector.hpp"
#include "COOMatrix.hpp"
#include "CSRMatrix.hpp"
#include "amath583.hpp"
#include "amath583IO.hpp"
#include "amath583sparse.hpp"

#ifdef TEST_CSC
#include "CSCMatrix.hpp"
#endif

#ifdef TEST_AOS
#include "AOSMatrix.hpp"
#endif

TEST_CASE("Identity", "[Eye]") {
  
  SECTION("COO") {
    Vector x(5);
    randomize(x);
    COOMatrix A = read_coomatrix("data/eye5.mmio");
    Vector y = A * x;
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(y(i) != 0.0);
      REQUIRE(y(i) == x(i));
    }
  }
  
  SECTION("CSR") {
    Vector x(5);
    randomize(x);
    CSRMatrix A = read_csrmatrix("data/eye5.mmio");
    Vector y = A * x;
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(y(i) != 0.0);
      REQUIRE(y(i) == x(i));
    }
  }

#ifdef TEST_CSC
  SECTION("CSC") {
    Vector x(5);
    randomize(x);
    CSCMatrix A = read_cscmatrix("data/eye5.mmio");
    Vector y = A * x;
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(y(i) != 0.0);
      REQUIRE(y(i) == x(i));
    }
  }
#endif

#ifdef TEST_AOS
  SECTION("AOS") {
    Vector x(5);
    randomize(x);
    AOSMatrix A = read_aosmatrix("data/eye5.mmio");
    Vector y = A * x;
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(y(i) != 0.0);
      REQUIRE(y(i) == x(i));
    }
  }  
#endif
}

TEST_CASE("Sherman", "[Sherman]") {

  COOMatrix ACOO = read_coomatrix("data/sherman4.mtx");    
  CSRMatrix ACSR = read_csrmatrix("data/sherman4.mtx");    

#ifdef TEST_CSC
  CSCMatrix ACSC = read_cscmatrix("data/sherman4.mtx");    
#endif

#ifdef TEST_AOS
  AOSMatrix AAOS = read_aosmatrix("data/sherman4.mtx");    
#endif

  SECTION("I/O") {
    REQUIRE(ACOO.num_rows() == ACSR.num_rows());
    REQUIRE(ACOO.num_cols() == ACSR.num_cols());    

#ifdef TEST_CSC
    REQUIRE(ACOO.num_rows() == ACSC.num_rows());
    REQUIRE(ACOO.num_cols() == ACSC.num_cols());    
#endif

#ifdef TEST_AOS
    REQUIRE(ACOO.num_rows() == AAOS.num_rows());
    REQUIRE(ACOO.num_cols() == AAOS.num_cols());    
#endif
  }

  SECTION("zero x") {

    Vector x(ACOO.num_cols());

    Vector yCOO = ACOO * x;
    Vector yCSR = ACSR * x;    

    REQUIRE(two_norm(yCOO-yCSR) < 1.e-9);

#ifdef TEST_CSC
    Vector yCSC = ACSC * x;
    REQUIRE(two_norm(yCOO-yCSC) < 1.e-9);
#endif

#ifdef TEST_AOS
    Vector yAOS = AAOS * x;    
    REQUIRE(two_norm(yCOO-yAOS) < 1.e-9);    
#endif
  }

  SECTION("Random x") {
    Vector x(ACOO.num_cols());
    randomize(x);

    Vector yCOO = ACOO * x;
    Vector yCSR = ACSR * x;    
    REQUIRE(two_norm(yCOO-yCSR) < 1.e-9);

#ifdef TEST_CSC
    Vector yCSC = ACSC * x;
    REQUIRE(two_norm(yCOO-yCSC) < 1.e-9);
#endif

#ifdef TEST_AOS
    Vector yAOS = AAOS * x;    
    REQUIRE(two_norm(yCOO-yAOS) < 1.e-9);    
#endif
  }
}
