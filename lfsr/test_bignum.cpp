#include <algorithm>

#include <catch2/catch_test_macros.hpp>

#include "bignum.h"

TEST_CASE("a default constructed bignum is all zeros")
{
  BigNum<1234> big;

  for (std::size_t i = 0; i < big.bitcount(); ++i) {
    REQUIRE(big.ith_bit(i) == false);
  }

  REQUIRE(big.popcount() == 0);
}

TEST_CASE("a specific bit can be set")
{
  BigNum<1234> big;

  constexpr std::size_t bitstoset[] = { 0, 78, 79, 1000 };
  for (const auto i : bitstoset) {
    big.set_bit_to(i, true);
  }
  REQUIRE(big.popcount() == std::size(bitstoset));

  for (const auto i : bitstoset) {
    if (std::ranges::any_of(bitstoset,
                            [i](auto needle) { return needle == i; })) {
      // bit should be set
      REQUIRE(big.ith_bit(i) == true);
    } else {
      // bit should not be set
      REQUIRE(big.ith_bit(i) == false);
    }
  }

  REQUIRE(big.parity(std::index_sequence<2, 3, 34>{}) == 0);
  REQUIRE(big.parity(std::index_sequence<2, 3, 34, 79>{}) == 1);
  REQUIRE(big.parity(std::index_sequence<2, 3, 34, 78, 79>{}) == 0);
  REQUIRE(big.parity(std::index_sequence<2, 3, 34, 78, 79, 80>{}) == 0);
}

TEST_CASE("bignum can be used in constexpr context")
{
  constexpr BigNum<1234> big;
  static_assert(big.bitcount() == 1234);
  static_assert(big.ith_bit<72>() == false);
  static_assert(big.ith_bit(72) == false);
  constexpr std::index_sequence<2, 3, 34, 78, 79, 80> seq{};
  static_assert(big.parity(seq) == 0);
}

template<std::size_t N>
void
verifyBitcounts()
{
  using Big = BigNum<N>;
  Big big;
  const auto bitsperlimb = Big::BitsPerLimb;
  const auto excessbits = Big::ExcessBits;
  const auto nlimbs = Big::LimbCount;
  REQUIRE(nlimbs > 0);
  REQUIRE(bitsperlimb > 0);
  REQUIRE(nlimbs * bitsperlimb >= N);
  REQUIRE(nlimbs * bitsperlimb == N + excessbits);

  REQUIRE(big.popcount() == 0);
  big.complement_in_place();
  REQUIRE(big.popcount() == N);
}

TEST_CASE("bit counts sum up to the expected amount")
{
  verifyBitcounts<2>();
  verifyBitcounts<3>();

  verifyBitcounts<7>();
  verifyBitcounts<8>();
  verifyBitcounts<9>();

  verifyBitcounts<15>();
  verifyBitcounts<16>();
  verifyBitcounts<17>();

  verifyBitcounts<31>();
  verifyBitcounts<32>();
  verifyBitcounts<33>();

  verifyBitcounts<63>();
  verifyBitcounts<64>();
  verifyBitcounts<65>();

  verifyBitcounts<127>();
  verifyBitcounts<128>();
  verifyBitcounts<129>();

  verifyBitcounts<255>();
  verifyBitcounts<256>();
  verifyBitcounts<257>();

  verifyBitcounts<511>();
  verifyBitcounts<512>();
  verifyBitcounts<513>();

  verifyBitcounts<1023>();
  verifyBitcounts<1024>();
  verifyBitcounts<1025>();

  verifyBitcounts<2047>();
  verifyBitcounts<2048>();
  verifyBitcounts<2049>();
}

TEST_CASE("a bignum can be bit flipped")
{
  BigNum<1234> big;

  big.set_bit_to(42, true);
  REQUIRE(big.popcount() == 1);

  big.complement_in_place();
  REQUIRE(big.popcount() == 1234 - 1);

  for (std::size_t i = 0; i < big.bitcount(); ++i) {
    if (i == 42) {
      REQUIRE(big.ith_bit(i) == false);
    } else {
      REQUIRE(big.ith_bit(i) == true);
    }
  }
}

TEST_CASE("a specific bit can be cleared")
{
  BigNum<1234> big;
  big.complement_in_place();

  const std::size_t bitstoclear[] = { 0, 78, 79, 1000 };
  for (const auto i : bitstoclear) {
    big.set_bit_to(i, false);
  }

  for (const auto i : bitstoclear) {
    if (std::ranges::any_of(bitstoclear,
                            [i](auto needle) { return needle == i; })) {
      // bit should not be set
      REQUIRE(big.ith_bit(i) == false);
    } else {
      // bit should be set
      REQUIRE(big.ith_bit(i) == true);
    }
  }
}

TEST_CASE("a bignum can be right shifted one bit")
{
  for (int bit : { 25, 78, 79, 1233 }) {
    BigNum<1234> big;
    big.set_bit_to(bit, true);
    big.shr_one_bit();
    REQUIRE(big.ith_bit(bit - 0) == false);
    REQUIRE(big.ith_bit(bit - 1) == true); // <---- the bit that was set
    REQUIRE(big.ith_bit(bit - 2) == false);
    REQUIRE(big.popcount() == 1);
  }

  // make an all ones, which we shift
  BigNum<1234> big;
  big.complement_in_place();
  for (std::size_t i = 0; i < 1234; ++i) {
    REQUIRE(big.popcount() == 1234 - i);
    big.shr_one_bit();
  }
  REQUIRE(big.popcount() == 0);
}

/* no need to provide this
TEST_CASE("a bignum can be compared with == and !=")
{
  BigNum<1234> a;
  BigNum<1234> b;
  REQUIRE(a == b);
  b.set_bit_to(12, true);
  REQUIRE(a != b);
}
*/

TEST_CASE("a bignum can be converted to uint64")
{
  BigNum<1234> a;

  GIVEN("no bits set ")
  {
    const auto b = to_uint64(a);
    REQUIRE(b == 0);
  }

  GIVEN("low bits set ")
  {
    a.set_bit_to(10, true);
    const auto b = to_uint64(a);
    REQUIRE(b == 1024);
  }

  GIVEN("high bits set ")
  {
    a.set_bit_to(50, true);
    const auto b = to_uint64(a);
    REQUIRE(b == (1ULL << 50));
  }

  GIVEN("high and low bits set ")
  {
    a.set_bit_to(10, true);
    a.set_bit_to(50, true);
    const auto b = to_uint64(a);
    REQUIRE(b == (1ULL << 10) + (1ULL << 50));
  }
}
