
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test module name
#include <boost/test/unit_test.hpp>

#include "FloatingPoint.hpp"

BOOST_AUTO_TEST_CASE( construct_from_small_ints ) {
    using F=Floating;
    for(int i=-10; i<=10; ++i) {
        const auto x=F::FromInt(i);
        BOOST_TEST(x.ToInt()==i);
    }
}

BOOST_AUTO_TEST_CASE( construct_from_problematic_ints ) {
    using F=Floating;
    using L=std::numeric_limits<std::int64_t>;
    for(std::int64_t  i: {L::min(),L::min()+1,L::min()+2,L::max()-2,L::max()-1,L::max()}) {
        const auto x=F::FromInt(i);
        BOOST_TEST(x.ToInt()==i);
    }
}

BOOST_AUTO_TEST_CASE( construct_from_positive_powers_of_two ) {
    using F=Floating;
    for(int exponent=0; exponent<=62; ++exponent) {
        const auto x=F::FromMantissaAndExp(1,exponent);
        BOOST_TEST(x.ToInt()==(1LL<<exponent));
    }
}
