/*
 * This file demonstrates how to call a function once for each argument given,
 * while at the same time passing an index. It is easiest explained with an
 * example:
 *
 * invoke_with_index(f,a,b,c);
 *
 * will be translated to:
 *
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

namespace detail {
template<class F,class ...Args, std::size_t ... I>
inline void invoke_with_index(F&& f, std::integer_sequence<unsigned long, I...>,
    const Args& ...args) {
  std::initializer_list<int> { (f(I+1,args),0)... };
}
}

template<class F,class ...Args>
inline void invoke_with_index(F&&f, const Args& ... args) {
  detail::invoke_with_index(std::forward<F>(f),std::index_sequence_for<Args...>(), args...);
}

int main() {

  auto f=[](std::size_t index, const auto& value) {
    std::cout << "f(" << index << ", " << value << ") with T="
        << boost::core::demangle(typeid(value).name()) << "\n";
  };

  invoke_with_index(f, 1.2, "hej", 4);

  return 0;
}
