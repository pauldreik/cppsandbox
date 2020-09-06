#include <utility>
#include <array>
#include <iostream>
#include <cmath>
#include <ratio>
#include <cassert>


// calculates cos( pi* Num/Den)
template<std::intmax_t Num, std::intmax_t Den>
constexpr double mycos(std::ratio<Num,Den> x) {
    using std::ratio;
    using std::ratio_greater_equal_v;
    using std::is_same_v;

    // simplify the ratio and remove excess rotation
    constexpr auto den=x.den;
    static_assert(den>0);
    constexpr auto num_wo_rot=x.num%(2*den);
    constexpr auto num=((num_wo_rot<0)?num_wo_rot+2*den:num_wo_rot);
    using S=ratio<num,den>;

    // 0<=S<2 at this point
    static_assert(num>=0);
    static_assert(num<2*den);

    using pi=ratio<1>;
    using pi_half=ratio<1,2>;

    constexpr bool den_is_power_of_two = ((den & (den - 1)) == 0);
    constexpr bool den_is_multiple_of_two = (den&0b1)==0;

    if constexpr(num==0) {
        // cos(0)=1
        return 1.0;
    }

    else if constexpr(std::is_same_v<S, ratio<1,3>>) {
        // cos(pi/3)=0.5
        return 0.5;
    }
    else if constexpr(std::is_same_v<S, ratio<1,5>>) {
        // cos(pi*2/5)=(sqrt(5)+1)/4
        return (std::sqrt(5.0)+1)*0.25;
    }
    else if constexpr(std::is_same_v<S, ratio<2,5>>) {
        // cos(pi*2/5)=(sqrt(5)-1)/4
        return (std::sqrt(5.0)-1)*0.25;
    }
    else if constexpr(std::is_same_v<S, pi_half>) {
        // cos(pi/2)=0
        return 0.0;
    }
    else if constexpr(ratio_greater_equal_v<S,pi>) {
        //cos(x)=-cos(x-pi)
        return -mycos(std::ratio_subtract<S,pi>{});
    }

    // 0<S<1 at this point
    else if constexpr(ratio_greater_equal_v<S,pi_half>) {
        //cos(x)=-cos(pi-x)
        return -mycos(std::ratio_subtract<pi,S>{});
    }

    // 0<S<1/2 at this point - first quadrant!
    else if constexpr(num==1 && den_is_multiple_of_two) {
        // see if the angle is on the form fi/2
        // we can use https://en.wikipedia.org/wiki/Exact_trigonometric_constants#Calculated_trigonometric_values_for_sine_and_cosine
        double cos_2x=mycos(ratio<2*num,den>{});
        return 0.5*std::sqrt(2+2*cos_2x);
    } else {
        // if we come here, more special cases are needed
        throw "needs more special cases";
    }
}

namespace detail {
template<std::intmax_t first,class What,std::intmax_t... Indices>
constexpr auto static_for_detail(What w,std::integer_sequence<std::intmax_t,Indices...>) {
    [[maybe_unused]] auto unused={((w(std::integral_constant<std::intmax_t,first+Indices>{}),0),...)};
}
}

template<std::intmax_t first,
         std::intmax_t last,
         class What>
constexpr void static_for(What w) {
    static_assert(first<=last);
    detail::static_for_detail<first>(w,std::make_integer_sequence<std::intmax_t,last-first>());
}

auto square(double x) { return x*x;}

template<std::intmax_t Num, std::intmax_t Den>
void verify(std::ratio<Num,Den> x) {
    const double truth=std::cos(Num*M_PI/Den);
    const double actual=mycos(x);
    assert(std::abs(truth-actual)<1e-15);
}

int main() {
    using std::ratio;
    assert(mycos(ratio<0,1>())==1);
    assert(mycos(ratio<0,2>())==1);
    assert(mycos(ratio<0,3>())==1);
    assert(mycos(ratio<1,2>())==0);
    assert(mycos(ratio<-1,2>())==0);
    assert(mycos(ratio<1,3>())==0.5);
    verify(ratio<1,3>{});
    verify(ratio<2,3>{});
    verify(ratio<3,3>{});
    verify(ratio<4,3>{});
    verify(ratio<4,4>{});

    static_for<1,7>([](auto den){
        static_for<-10,10>([&](auto num){
            verify(ratio<num.value,den.value>{});
        });
    });
}
