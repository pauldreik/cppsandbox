
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

BOOST_AUTO_TEST_CASE( unary_minus_edgecases ) {
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
            if(negx.ToInt().has_value() && x.ToInt().has_value()) {
                BOOST_TEST(x.ToInt().value()==-negx.ToInt().value());
            }
        }
    }
}


BOOST_AUTO_TEST_CASE( plus_of_small_values) {
    auto zero=Floating::FromInt(0);
    auto one=Floating::FromInt(1);
    auto two=Floating::FromInt(2);
    BOOST_TEST((zero+one).ToInt().value()==1L);
    BOOST_TEST((one+zero).ToInt().value()==1L);
    BOOST_TEST((one+two).ToInt().value()==3L);
    BOOST_TEST((one+two+zero).ToInt().value()==3L);
    BOOST_TEST((one+-one).ToInt().value()==0L);
    for(int a=-2; a<=2; ++a) {
        for(int b=-2; b<=2; ++b) {
            BOOST_TEST((Floating::FromInt(a)+Floating::FromInt(b)).ToInt().value()==a+b);
        }
    }
}

BOOST_AUTO_TEST_CASE (plus_of_weird_values) {
    using F=Floating;
    using L=std::numeric_limits<std::int64_t>;
    const auto mantissas={L::min(),
                          L::min()+1,L::min()+2,-2L,-1L,0L,1L,2L,L::max()-2,L::max()-1,L::max()};
    const auto exps={-129,-128,-127,-66,-65,-64,-63,-62,-61,-60,
                     -2,-1,0,1,2,60,61,62,63,64,65,66,127,128,129};
    const auto zero=F::FromInt(0);
    for(std::int64_t i1: mantissas) {
        for(int exp1 : exps) {
            const auto a=F::FromMantissaAndExp(i1,exp1);
            if(!(a==a+zero)) {
                std::abort();
            }
            if(!(zero==a+-a)) {
                std::abort();
            }
            for(std::int64_t i2: mantissas) {
                for(int exp2 : exps) {
                    auto b=F::FromMantissaAndExp(i2,exp2);
                    const auto aplusb=a+b;
                    if(!(aplusb==b+a)) {
                        std::abort();
                    }
                    if(a.isPositive() && b.isPositive() && !aplusb.isPositive())
                        std::abort();
                    if(a.isNegative() && b.isNegative() && !aplusb.isNegative())
                        std::abort();

                    //see if it is representable as ints, then verify with ordinary ints.
                    if(a.ToInt().has_value() && b.ToInt().has_value() && aplusb.ToInt().has_value()) {
                        //promote to a larger type to avoid UB
                        __int128 a128=a.ToInt().value();
                        __int128 b128=b.ToInt().value();
                        __int128 aplusb128=aplusb.ToInt().value();
                        //only check if precision could have been preserved
                        __int128 exact_answer=a128+b128;
                        if(a128>L::min() && b128>L::min() && exact_answer >L::min()&& exact_answer<=L::max())
                            if((a128+b128)!=aplusb128)
                                std::abort();
                    }
                    BOOST_TEST_CHECKPOINT("all good");
                }
            }
        }
    }
    BOOST_CHECK("all good");
}

BOOST_AUTO_TEST_CASE( multiplication_small_values) {
    auto zero=Floating::FromInt(0);
    auto one=Floating::FromInt(1);
    auto two=Floating::FromInt(2);
    BOOST_TEST((zero*one).ToInt().value()==0L);
    BOOST_TEST((one*zero).ToInt().value()==0L);
    BOOST_TEST((one*two).ToInt().value()==2L);
    BOOST_TEST((one*-one).ToInt().value()==-1L);
    for(int a=-2; a<=2; ++a) {
        for(int b=-2; b<=2; ++b) {
            BOOST_TEST((Floating::FromInt(a)*Floating::FromInt(b)).ToInt().value()==a*b);
        }
    }
}


BOOST_AUTO_TEST_CASE (multiplication_of_weird_values) {
    using F=Floating;
    using L=std::numeric_limits<std::int64_t>;
    const auto mantissas={L::min(),
                          L::min()+1,L::min()+2,-2L,-1L,0L,1L,2L,L::max()-2,L::max()-1,L::max()};
    const auto exps={-129,-128,-127,-66,-65,-64,-63,-62,-61,-60,
                     -2,-1,0,1,2,60,61,62,63,64,65,66,127,128,129};
    const auto zero=F::FromInt(0);
    const auto one=F::FromInt(1);
    const auto two=F::FromInt(2);
    for(std::int64_t i1: mantissas) {
        for(int exp1 : exps) {
            const auto a=F::FromMantissaAndExp(i1,exp1);
            if(!(a==a*one)) {
                std::abort();
            }
            auto apa=a+a;
            auto twoa=two*a;
            if(!(twoa==apa)) {
                std::abort();
            }
            for(std::int64_t i2: mantissas) {
                for(int exp2 : exps) {
                    auto b=F::FromMantissaAndExp(i2,exp2);
                    if(!(a*b==b*a)) {
                        std::abort();
                    }
                    const auto ab=a*b;
                    if(!(a.sign()*b.sign()==ab.sign()))
                        std::abort();
                    BOOST_TEST_CHECKPOINT("all good");
                }
            }
        }
    }
    BOOST_CHECK("all good");
}
