#include <catch2/catch_test_macros.hpp>

#include "integerselect.h"

TEST_CASE("the correct integer is selected")
{
  // will fail to compile
  // static_assert(std::is_same_v<SelectInteger_t<-1>, std::uint8_t>);

  static_assert(std::is_same_v<SelectInteger_t<0>, std::uint8_t>);
  static_assert(std::is_same_v<SelectInteger_t<1>, std::uint8_t>);
  static_assert(std::is_same_v<SelectInteger_t<8>, std::uint8_t>);
  static_assert(std::is_same_v<SelectInteger_t<9>, std::uint16_t>);
  static_assert(std::is_same_v<SelectInteger_t<16>, std::uint16_t>);
  static_assert(std::is_same_v<SelectInteger_t<17>, std::uint32_t>);
  static_assert(std::is_same_v<SelectInteger_t<32>, std::uint32_t>);
  static_assert(std::is_same_v<SelectInteger_t<31>, std::uint32_t>);
  static_assert(std::is_same_v<SelectInteger_t<32>, std::uint32_t>);
  static_assert(std::is_same_v<SelectInteger_t<33>, std::uint64_t>);
  static_assert(std::is_same_v<SelectInteger_t<45>, std::uint64_t>);
  static_assert(std::is_same_v<SelectInteger_t<64>, std::uint64_t>);

  // will fail to compile
  // static_assert(std::is_same_v<SelectInteger_t<65>, std::uint64_t>);
}
