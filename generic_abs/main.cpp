#include <cstdint>
#include <limits>
#include <string>
#include <utility>

/**
 * generic_abs returns the absolute value of a signed integer,
 * without invoking undefined behaviour.
 * @return the absolute value of the input, as an unsigned integer of the same
 * size as the input
 */
template<std::signed_integral T>
[[nodiscard]] constexpr auto
generic_abs(T x)
{
  using U = std::make_unsigned_t<T>;
  const U tmp = x;
  // Imagine x is negative (mathematically: -v where v>0) and T is int. Then U
  // is unsigned int. By two complement rules and assuming 32 bits in this
  // example, tmp==2**32-v. Negating tmp follows the rule in the expr.unary
  // section of the C++ standard which results in -tmp==2**32-(2**32-v) which is
  // v. So we get the absolute value v we wanted. See
  // https://eel.is/c++draft/expr.unary#op-8
  return static_cast<U>(x < 0 ? -tmp : tmp);
};

//
//
// the rest of this file is just unit testing
//
//

template<std::integral I>
  requires(!std::is_const_v<I> && !std::is_reference_v<I> &&
           !std::is_volatile_v<I> && std::is_signed_v<I> &&
           !std::is_same_v<I, bool>)
constexpr bool
check_correctness()
{
  using U = std::make_unsigned_t<I>;

  static_assert(
    std::is_same_v<decltype(generic_abs(I{})), U>,
    "the return type must be the unsigned version of the input integer type I");

  static_assert(generic_abs(I{ -1 }) == U{ 1 });
  static_assert(generic_abs(I{ 0 }) == U{ 0 });
  static_assert(generic_abs(I{ 1 }) == U{ 1 });

  if constexpr (sizeof(I) >= sizeof(std::int8_t)) {
    static_assert(generic_abs(I{ 127 }) == U{ 127 });
    static_assert(generic_abs(I{ -128 }) == U{ 128 });
  }

  if constexpr (sizeof(I) >= sizeof(std::int16_t)) {
    static_assert(generic_abs(I{ 32767 }) == U{ 32767 });
    static_assert(generic_abs(I{ -32768 }) == U{ 32768 });
  }

  if constexpr (sizeof(I) >= sizeof(std::int32_t)) {
    static_assert(generic_abs(I{ 2147483647 }) == U{ 2147483647 });
    static_assert(generic_abs(I{ -2147483648 }) == U{ 2147483648 });
  }

  if constexpr (sizeof(I) >= sizeof(std::int64_t)) {
    static_assert(generic_abs(I{ 9223372036854775807 }) ==
                  U{ 9223372036854775807 });
    static_assert(generic_abs(I{ -9223372036854775807LL - 1 }) ==
                  U{ 9223372036854775808ULL });
  }

  // the signed max should work correctly
  constexpr I max = std::numeric_limits<I>::max();
  static_assert(generic_abs(I{ max }) == U{ max });

  // the signed min should work correctly
  constexpr I min = std::numeric_limits<I>::min();
  constexpr U abs_min_as_U{ U{ max } + U{ 1 } };
  static_assert(generic_abs(min) == abs_min_as_U);

  return true;
}

template<typename T>
concept generic_abs_invokable = requires(T x) { generic_abs(x); };

template<typename T>
  requires requires(T x, const T cx, const T& cxref, T& xref) {
    generic_abs(x);
    generic_abs(cx);
    generic_abs(cxref);
    generic_abs(xref);
    { generic_abs(x) } -> std::same_as<std::make_unsigned_t<T>>;
    { generic_abs(cx) } -> std::same_as<std::make_unsigned_t<T>>;
    { generic_abs(std::move(x)) } -> std::same_as<std::make_unsigned_t<T>>;
  }
constexpr bool
check_valid_type()
{
  static_assert(check_correctness<T>());
  return true;
}

enum class scoped_enum
{
  value
};

enum classic_enum
{
  value
};

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  static_assert(check_valid_type<std::int8_t>());
  static_assert(check_valid_type<std::int16_t>());
  static_assert(check_valid_type<std::int32_t>());
  static_assert(check_valid_type<std::int64_t>());

  static_assert(generic_abs_invokable<std::int8_t>);
  static_assert(generic_abs_invokable<std::int16_t>);
  static_assert(generic_abs_invokable<std::int32_t>);
  static_assert(generic_abs_invokable<std::int64_t>);

  static_assert(!generic_abs_invokable<unsigned char>);
  static_assert(!generic_abs_invokable<unsigned short>);
  static_assert(!generic_abs_invokable<unsigned int>);
  static_assert(!generic_abs_invokable<unsigned long>);
  static_assert(!generic_abs_invokable<unsigned long long>);

  static_assert(!generic_abs_invokable<bool>);
  static_assert(!generic_abs_invokable<float>);
  static_assert(!generic_abs_invokable<double>);
  static_assert(!generic_abs_invokable<std::string>);
  static_assert(!generic_abs_invokable<int*>);
  static_assert(!generic_abs_invokable<scoped_enum>);
  static_assert(!generic_abs_invokable<classic_enum>);
}
