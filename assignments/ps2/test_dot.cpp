#define CATCH_CONFIG_MAIN    // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Vector.hpp"
#include "amath583.hpp"

TEST_CASE("Dot product", "[Dot]") {
  Vector x(50), y(50);
  REQUIRE(x.num_rows() == 50);
  REQUIRE(y.num_rows() == 50);

  SECTION("test initial values") {
    for (size_t i = 0; i < 50; ++i) {
      x(i) = y(i) = 1.0;
    }
    for (size_t i = 0; i < 50; ++i) {
      REQUIRE((1.0 == x(i) && 1.0 == y(i)));
    }
    REQUIRE(dot(x, y) == 50);
    for (size_t i = 0; i < 50; ++i) {
      x(i) = y(i) = 2.0;
    }
    for (size_t i = 0; i < 50; ++i) {
      REQUIRE((2.0 == x(i) && 2.0 == y(i)));
    }
    REQUIRE(dot(x, y) == 200);
  }
}  
