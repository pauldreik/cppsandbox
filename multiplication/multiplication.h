#pragma once

#include <cstdint>

/**
 * multiplies x and y modulo a_i, while avoiding
 * internal overflow
 * @return xy mod a_i
 */
template<std::uint32_t a_i>
constexpr std::uint32_t
multiply_modulo(const std::uint32_t x, const std::uint32_t y)
{
  const auto xm = x % a_i;
  const auto ym = y % a_i;
  return (xm * ym) % a_i;
}

/**
 * returns true if a*b == c*d, determined while avoiding
 * overflow
 * @param a
 * @param b
 * @param c
 * @param d
 * @return
 */
constexpr bool
products_equal(const std::uint32_t a,
               const std::uint32_t b,
               const std::uint32_t c,
               const std::uint32_t d)
{
  // we use the chinese remainder theorem with four coefficients

  // carry out the comparision modulo a_1=2**32 which is fast
  if (a * b != c * d) {
    return false;
  }
  // this coefficient was chosen after benchmarking
  constexpr std::uint32_t a_2 = 65485;
  if (multiply_modulo<a_2>(a, b) != multiply_modulo<a_2>(c, d)) {
    return false;
  }

  // this coefficient was chosen after benchmarking
  constexpr std::uint32_t a_3 = 65483;
  if (multiply_modulo<a_3>(a, b) != multiply_modulo<a_3>(c, d)) {
    return false;
  }

  // this coefficient was chosen after benchmarking
  constexpr std::uint32_t a_4 = 65481;
  if (multiply_modulo<a_4>(a, b) != multiply_modulo<a_4>(c, d)) {
    return false;
  }

  return true;
}
