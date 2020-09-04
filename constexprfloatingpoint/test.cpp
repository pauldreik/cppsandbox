
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test module name
#include <boost/test/unit_test.hpp>

#include "FloatingPoint.hpp"



BOOST_AUTO_TEST_CASE( construct_from_small_ints ) {
    using F=Floating;
    for(int i=-10; i<=10; ++i) {
        const auto x=F::FromInt(i);
        BOOST_TEST(x.ToInt().value()==i);
    }
    BOOST_TEST(F::FromInt(0).isZero());
    BOOST_TEST(!F::FromInt(1).isZero());
    BOOST_TEST(!F::FromInt(-1).isZero());
}

BOOST_AUTO_TEST_CASE( construct_from_problematic_ints ) {
    using F=Floating;
    using L=std::numeric_limits<std::int64_t>;
    // all these should be possible to recover without loss
    const auto min=L::min();
    const auto max=L::max();
    using I=std::int64_t;
    for(std::int64_t  i: {min,min+1,min+2,I{-2},I{-1},I{0},I{1},I{2},max-2,max-1,max}) {
        const auto x=F::FromInt(i);
        BOOST_TEST(x.ToInt().value()==i);
    }
}
BOOST_AUTO_TEST_CASE( normalize ) {
    using F=Floating;
    const auto x=F::FromInt(0);
    BOOST_TEST(Floating{x}.normalize().ToInt().value()==x.ToInt().value());
}
BOOST_AUTO_TEST_CASE( construct_from_positive_powers_of_two ) {
    using F=Floating;
    for(int exponent=0; exponent<=62; ++exponent) {
        const auto x=F::FromMantissaAndExp(1,exponent);
        BOOST_TEST(x.ToInt().value()==(1LL<<exponent));
    }
}

BOOST_AUTO_TEST_CASE( unary_minus ) {
    using F=Floating;
    using L=std::numeric_limits<std::int64_t>;
    for(std::int64_t  i: {L::min(),
        L::min()+1,L::min()+2,-2L,-1L,0L,1L,2L,L::max()-2,L::max()-1,L::max()}) {
        for(int exp=-2; exp<=2; ++exp) {
            const auto x=F::FromMantissaAndExp(i,exp);
            const auto negx=-x;
            //zero should still be zero
            BOOST_TEST(x.isZero()==negx.isZero());

            if(!x.isZero()) {
                BOOST_TEST(x.isNegative()==!x.isPositive());
                BOOST_TEST(x.isNegative()==!negx.isNegative());
                BOOST_TEST(x.isPositive()==!negx.isPositive());
            }
        }
    }
}
