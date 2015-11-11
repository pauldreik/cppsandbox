#include <iostream>
#include <iomanip>
#include <limits>
#include <typeinfo>
#include <cstdint>
#include <type_traits>

#include <boost/version.hpp>
#if BOOST_VERSION >= 105600
#include <boost/core/demangle.hpp>
#endif

namespace IntegerSelector {
namespace detail {
//traits class - Index 0 and up picks larger and larger integers, note
//that it works even if they would be equal (say least8 and least16 are both 32 bit)
template<std::size_t Index>
struct IntegerSiblings { };
template<>
struct IntegerSiblings<0> {
	using Myself=std::uint_least8_t;
};
template<>
struct IntegerSiblings<1> {
	using Myself=std::uint_least16_t;
};
template<>
struct IntegerSiblings<2> {
	using Myself=std::uint_least32_t;
};
template<>
struct IntegerSiblings<3> {
	//guaranteed to exist.
	using Myself=std::uint_least64_t;
};
//in case 128 bit integers show up, it would have to be added
//as a specialization here.

template<std::uintmax_t value, std::size_t Index = 0>
struct PickOne {
	//which integer type do we have at this level of recursion?
	using Sibling=IntegerSiblings<Index>;

	//is that type too small?
	static const bool First_is_too_small = //
			(std::numeric_limits<typename Sibling::Myself>::max() <= value);

	//if so recurse into a bigger type. otherwise, be happy as is.
	using type=typename std::conditional<First_is_too_small,
	typename PickOne<value,Index+1>::type,
	typename Sibling::Myself>::type;
};

//specialization for when the maximum size is reached
template<std::uintmax_t value>
struct PickOne<value, 4> {
	using type=std::uintmax_t;
};
}//namespace detail

/*
 * this is the users entry - value is the maximum range
 * to cover (inclusive).
 */
template<std::uintmax_t value>
using SmallestUintCapableOf=typename detail::PickOne<value,0>::type;
} //namespace IntegerSelector

/*
 * test code - to nicely display how it works.
 */
template<std::uintmax_t value>
void showInfo(std::ostream& os=std::cout) {
	//this is the chosen type large enough to hold "value".
	using IntegerType=typename IntegerSelector::SmallestUintCapableOf<value>;

	os<<"To cover "<<std::setw(std::numeric_limits<std::uintmax_t>::digits10+1)<<value<<" the type needs to be ";
	  char const * name = typeid(IntegerType).name();
	#if BOOST_VERSION >= 105600
	  os << boost::core::demangle(name);
	#else
	  os << name;
	#endif
	  os<<"\n";
}

int main(int /*argc*/, char* /*argv*/[]) {

	/*
	 * say we are faced with the following task - we need to store a value,
	 * whose maximum range we know. we now want to use the smallest type
	 * capable of storing any value in that range. If we are making
	 * a calendar application, it could look like this:
	 */
	struct CalendarEntry {
		using Year=IntegerSelector::SmallestUintCapableOf<3000>;
		using Month=IntegerSelector::SmallestUintCapableOf<12>;
		using Day=IntegerSelector::SmallestUintCapableOf<31>;
		using DayOfYear=IntegerSelector::SmallestUintCapableOf<366>;

		Year m_year;
		Month m_month;
		Day m_day;
		DayOfYear whichDayIsIt() const;
	};
	/*
	 * maybe you say that is silly, because we can use the std::uintXX_t from the
	 * standard but that may not work:
	 * - the standard does not mandate to supply the exact width
	 * - even if the mandatory uint_least_XX typedefs are used, the requested range
	 *   may not be known at the time of writing - maybe because we are writing
	 *   a template for general use.
	 */

	//for fun, lets see which types are needed for these values
	showInfo<1>();
	showInfo<1000>();
	showInfo<1000000>();
	showInfo<1000000000>();
	showInfo<1000000000000>();
	showInfo<1000000000000000>();
	showInfo<1000000000000000000>();
	showInfo<std::numeric_limits<std::uintmax_t>::max()>();
	return 0;
}

