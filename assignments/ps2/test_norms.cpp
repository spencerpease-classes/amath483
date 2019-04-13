#define CATCH_CONFIG_MAIN    // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Vector.hpp"
#include "amath583.hpp"


TEST_CASE("Vectors can be created and initialized", "[Vector]") {
  Vector x(50);
  REQUIRE(x.num_rows() == 50);

  SECTION("test initial values") {
    for (size_t i = 0; i < 50; ++i) {
      REQUIRE(x(i) == 0.0);
    }
  }

  SECTION("test filling values") {
    for (size_t i = 0; i < 50; ++i) {
      x(i) = 50 - i;
    }
    for (size_t i = 0; i < 50; ++i) {
      REQUIRE(x(i) == 50 - i);
    }
    zeroize(x);
    for (size_t i = 0; i < 50; ++i) {
      REQUIRE(x(i) == 0.0);
    }
  }
}  


TEST_CASE("Take one norm of a Vector", "[One Norm]") {
  Vector x(50);
  SECTION("one_norm") {
    zeroize(x);
    REQUIRE(one_norm(x) == 0.0);
    x(0) = 1;
    REQUIRE(one_norm(x) == 1.0);
    x(49) = x(39) = x(11) = 1.0;
    REQUIRE(one_norm(x) == 4.0);
  }
}


TEST_CASE("Take two norm of a Vector", "[Two Norm]") {
  Vector x(50);
  SECTION("two_norm") {
    zeroize(x);
    REQUIRE(two_norm(x) == 0.0);
    x(0) = 1;
    REQUIRE(two_norm(x) == 1.0);
    x(49) = x(39) = x(11) = 1.0;
    REQUIRE(two_norm(x) == 2.0);
  }
}


TEST_CASE("Take inf norm of a Vector", "[Inf Norm]") {
  Vector x(50);
  SECTION("inf_norm") {
    zeroize(x);
    REQUIRE(inf_norm(x) == 0.0);
    x(0) = 1;
    REQUIRE(inf_norm(x) == 1.0);
    x(49) = x(39) = x(11) = 1.0;
    REQUIRE(inf_norm(x) == 1.0);
  }
}
