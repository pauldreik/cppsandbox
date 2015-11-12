#include <iostream>
#include <typeinfo>
#include <utility>
#include <tuple>
#include <boost/version.hpp>
#if BOOST_VERSION >= 105600
#include <boost/core/demangle.hpp>
#endif

namespace reorganizer {
namespace detail {
template<typename ...Elems, std::size_t ...Indices>
auto reorganizeTuple(const std::tuple<Elems...>& tuple,
		typename std::integer_sequence<std::size_t, Indices...> /*bogus*/) {
	return std::make_tuple(std::get<Indices>(tuple)...);
}
}

template<std::size_t ...Indices, typename Tuple>
auto reOrganize(const Tuple& tuple) {
	return detail::reorganizeTuple(tuple,
			std::integer_sequence<std::size_t, Indices...>());
}

}
namespace {
template<typename In, std::size_t N = std::tuple_size<In>::value>
void printInfo(const In& /*in*/, const char* msg = nullptr) {
	std::ostream& os(std::cout);
	if (msg) {
		os << msg;
	}
	char const * inname = typeid(In).name();
#if BOOST_VERSION >= 105600
	os << boost::core::demangle(inname);
#else
	os << inname;
#endif
	os << "\n";
}
}

struct H {
};
struct E {
};
struct L {
};
struct O {
};
int main(int /*argc*/, char* /*argv*/[]) {

	/*
	 * demonstrates how to reorganize the elements of a tuple, at compile time.
	 */
	auto x = std::make_tuple(H(), E(), L(), L(), O());
	auto christmas = reorganizer::reOrganize<0, 4, 0, 4, 0, 4>(x);
	auto manager = reorganizer::reOrganize<4, 3, 2, 1>(x);

	printInfo(x, "original input:\t");
	printInfo(christmas, "reorganized:\t");
	printInfo(manager, "reorganized:\t");
	return 0;
}

