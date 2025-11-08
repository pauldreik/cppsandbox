#include <catch2/catch_all.hpp>

#include <multiplication.h>

TEST_CASE("multiply modulo")
{
  static_assert(multiply_modulo<3>(1, 1) == 1);
  static_assert(multiply_modulo<3>(0, 1) == 0);
  static_assert(multiply_modulo<3>(4, 1) == 1);
  static_assert(multiply_modulo<3>(1, 4) == 1);
  static_assert(multiply_modulo<3>(2, 4) == 2);
  static_assert(multiply_modulo<3>(4, 2) == 2);
}

TEST_CASE("zeros and ones")
{
  static_assert(products_equal(0, 0, 0, 0));
  static_assert(products_equal(0, 0, 0, 1));
  static_assert(products_equal(0, 0, 1, 0));
  static_assert(!products_equal(0, 0, 1, 1));
  static_assert(products_equal(0, 1, 0, 0));
  static_assert(products_equal(0, 1, 0, 1));
  static_assert(products_equal(0, 1, 1, 0));
  static_assert(!products_equal(0, 1, 1, 1));
  static_assert(products_equal(1, 0, 0, 0));
  static_assert(products_equal(1, 0, 0, 1));
  static_assert(products_equal(1, 0, 1, 0));
  static_assert(!products_equal(1, 0, 1, 1));
  static_assert(!products_equal(1, 1, 0, 0));
  static_assert(!products_equal(1, 1, 0, 1));
  static_assert(!products_equal(1, 1, 1, 0));
  static_assert(products_equal(1, 1, 1, 1));
}

TEST_CASE("interesting numbers")
{
  constexpr unsigned interesting_numbers[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    655531,
    655532,
    655533,
    655534,
    655535,
    655536,
    655537,
    655538,
    655539,
    655540,
    UINT_MAX - 5,
    UINT_MAX - 4,
    UINT_MAX - 3,
    UINT_MAX - 2,
    UINT_MAX - 1,
    UINT_MAX,
  };
  for (const unsigned a : interesting_numbers) {
    for (const unsigned b : interesting_numbers) {
      REQUIRE(products_equal(a, b, a, b));
      REQUIRE(products_equal(a, b, b, a));
      REQUIRE(products_equal(b, a, a, b));
      REQUIRE(products_equal(b, a, b, a));
    }
  }
}

TEST_CASE("hand picked counterexamples")
{
  static_assert(!products_equal(4291624960, 4291493888, 0, 0));

  // this example was found by my friend who asked ai, which generated
  // code for brute forcing for a counterexample.
  static_assert(
    !products_equal(4294940936U, 4289265232U, 372413899U, 15529856U));

  static_assert(!products_equal(4291624960, 4291493888, 0, 0));
  static_assert(!products_equal(4291362816, 4291624960, 3598, 0));
}
