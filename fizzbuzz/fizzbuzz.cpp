/*
* compile time fizzbuzz
* by Paul Dreik
* http://www.pauldreik.se
*/
#include <iostream>
#include <utility>
#include <string>
#include <typeinfo>

namespace P {

struct Fizz {};
struct Buzz {};
struct FizzBuzz {};
template<int I>
struct Number {};

template<int I, bool D3, bool D5>
struct BoxBase {
  using type=Number<I>;
};

template<int I>
struct BoxBase<I, true, false> {
  using type=Fizz;
};

template<int I>
struct BoxBase<I, false, true> {
  using type=Buzz;
};

template<int I>
struct BoxBase<I, true, true> {
  using type=FizzBuzz;
};

template<int I>
struct Box: BoxBase<I, (I % 3) == 0, (I % 5) == 0> {
  static inline const char* toChar() {
    return typeid(typename Box::type).name();
  }
};

template<int ...Indices>
void implementation(std::integer_sequence<int, Indices...>) {
  for (auto e : { Box<Indices+1>::toChar()... }) {
    std::cout << e << "\n";
  }
  // to get a tuple containing the fizzbuzz sequence instead:
  //auto x=std::tuple<typename Box<Indices+1>::type...>();
}
}

void playFizzBuzz() {
  const int N = 898;
  P::implementation(std::make_integer_sequence<int, N>());
}

