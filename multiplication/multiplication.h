#pragma once

template<unsigned M>
constexpr unsigned
modulo(unsigned x)
{
  return x % M;
}

/**
 * multiplies x and y modulo a_i, while avoiding
 * internal overflow
 * @return xy mod a_i
 */
template<unsigned a_i>
constexpr unsigned
multiply_modulo(const unsigned x, const unsigned y)
{
  const auto xm = modulo<a_i>(x);
  const auto ym = modulo<a_i>(y);
  return modulo<a_i>(xm * ym);
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
products_equal(const unsigned a,
               const unsigned b,
               const unsigned c,
               const unsigned d)
{
  // we use the chinese remainder theorem with four coefficients

  // carry out the comparision modulo a_1=2**32 which is fast
  if (a * b != c * d) {
    return false;
  }
  // this coefficient was chosen after benchmarking
  constexpr unsigned a_2 = 65485;
  if (multiply_modulo<a_2>(a, b) != multiply_modulo<a_2>(c, d)) {
    return false;
  }

  // this coefficient was chosen after benchmarking
  constexpr unsigned a_3 = 65483;
  if (multiply_modulo<a_3>(a, b) != multiply_modulo<a_3>(c, d)) {
    return false;
  }

  // this coefficient was chosen after benchmarking
  constexpr unsigned a_4 = 65481;
  if (multiply_modulo<a_4>(a, b) != multiply_modulo<a_4>(c, d)) {
    return false;
  }

  return true;

}
