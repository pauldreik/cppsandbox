/*
 * Playing around with parallelizing std::partial_sum
 * by Paul Dreik https://www.pauldreik.se/
 * LICENSE: http://www.boost.org/LICENSE_1_0.txt
 */
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "include/parpartial.h"
#include <type_traits>
#include <vector>

// the truth (from std)
template <class T> auto theTruth(T input) {
  std::partial_sum(std::cbegin(input), std::cend(input), std::begin(input));
  return input;
}

// my variants
template <class T> auto myImplementation(T input, int implementation) {
  switch (implementation) {
  case 1:

    whatever::par_partial_sum(std::cbegin(input), std::cend(input),
                              std::begin(input));
    break;

  case 2:
    whatever::par_partial_sum_v2(std::cbegin(input), std::cend(input),
                                 std::begin(input));
    break;

  case 3:
    whatever::par_partial_sum_async(std::cbegin(input), std::cend(input),
                                    std::begin(input));
    break;
  default:
    throw "oops";
  }

  return input;
}
namespace {
void testEmpty(int implementation) {
  const std::vector<unsigned> Z;
  auto expected = Z;
  auto truth = theTruth(Z);
  BOOST_REQUIRE(1);
  auto mine = myImplementation(Z, implementation);
  BOOST_REQUIRE(expected == truth);
  BOOST_REQUIRE(expected == mine);
  BOOST_REQUIRE(truth.size() == 0);
}
}

BOOST_AUTO_TEST_CASE(test_empty) {
  testEmpty(1);
  testEmpty(2);
  testEmpty(3);
}

namespace {
void testZeros(int implementation) {
  const std::vector<unsigned> Z(5);
  auto expected = Z;
  auto truth = theTruth(Z);
  BOOST_REQUIRE(1);
  auto mine = myImplementation(Z, implementation);
  BOOST_REQUIRE(expected == truth);
  BOOST_REQUIRE(expected == mine);
  BOOST_REQUIRE(truth.size() == 5);
}
}
BOOST_AUTO_TEST_CASE(test_with_zeros) {
  testZeros(1);
  testZeros(2);
  testZeros(3);
}

namespace {
void testOnes(int implementation) {
  const std::vector<unsigned> Z(5, 1);
  const std::vector<unsigned> expected{1, 2, 3, 4, 5};
  BOOST_REQUIRE(1);

  auto truth = theTruth(Z);
  BOOST_REQUIRE(1);
  auto mine = myImplementation(Z, implementation);
  BOOST_REQUIRE(expected == truth);
  BOOST_REQUIRE(expected == mine);
  BOOST_REQUIRE(truth.size() == 5);
}
}
BOOST_AUTO_TEST_CASE(test_with_ones) {
  testOnes(1);
  testOnes(2);
  testOnes(3);
}

namespace {
void testLargeOnes(int implementation) {
  const std::vector<unsigned> Z(500, 1);
  std::vector<unsigned> expected(500);
  for (std::size_t i = 0; i < Z.size(); ++i) {
    expected.at(i) = 1 + i;
  }
  BOOST_REQUIRE(1);

  auto truth = theTruth(Z);
  BOOST_REQUIRE(1);
  auto mine = myImplementation(Z, implementation);
  BOOST_REQUIRE(expected == truth);
  BOOST_REQUIRE(expected == mine);
  BOOST_REQUIRE(truth.size() == 500);
}
}
BOOST_AUTO_TEST_CASE(test_with_large_ones) {
  testLargeOnes(1);
  testLargeOnes(2);
  testLargeOnes(3);
}
