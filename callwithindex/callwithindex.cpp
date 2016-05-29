/*
 * this file demonstrates how to call a function once for each argument given,
 * while at the same time passing an index.
 *
 * invoke_with_index(a,b,c);
 *
 * will be translated to
 * f(1,a);
 * f(2,b);
 * f(3,c);
 *
 * by Paul Dreik 2016, www.pauldreik.se
 * license: boost 1.0
 *
 */
#include <iostream>
#include <utility>
#include <typeinfo>
#include <boost/core/demangle.hpp>

template<typename T>
void f(std::size_t index, const T& value) {
  std::cout << "f(" << index << ", " << value << ") with T="
      << boost::core::demangle(typeid(value).name()) << "\n";
}

namespace detail {
template<class ...Args, std::size_t ... I>
inline void invoke_with_index(std::integer_sequence<unsigned long, I...>,
    const Args& ...args) {
  std::initializer_list<int> { (f(I+1,args),0)... };
}
}

template<class ...Args>
inline void invoke_with_index(const Args& ... args) {
  detail::invoke_with_index(std::index_sequence_for<Args...>(), args...);
}

int main() {

  invoke_with_index(1.2, "hej", 4);

  return 0;
}
