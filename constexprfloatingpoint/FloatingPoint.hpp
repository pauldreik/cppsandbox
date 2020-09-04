#ifndef FLOATINGPOINT_HPP
#define FLOATINGPOINT_HPP

#include <cstdint>
#include <cmath>
#include <cassert>
#include <type_traits>

namespace notinstd {
// counts leading zeros. if x<0, counts leading zeros in -x.
int countl_zero(std::int64_t x) {
    if(x==0) {
        return 64;
    }
    if(x>0) {
        if constexpr(std::is_same_v<std::int64_t,long>)
        return __builtin_clzl(x);
        if constexpr(std::is_same_v<std::int64_t,long long>)
        return __builtin_clzll(x);
    }
    // watch out for overflow here
    return countl_zero(-x);
}
}

struct Floating {
    using Rep=std::int64_t;
    Rep mantissa;
    int exponent;
    void print() const {
        std::cout<<mantissa<<'*'<<"2^"<<exponent<<'\n';
    }
    constexpr static Floating FromMantissaAndExp(std::int64_t mantissa,int exp) {return {mantissa,exp};}
    constexpr static Floating FromInt(std::int64_t x) {
        // FIXME: if x is intmin, shift it
        return {x,0};
    }
    constexpr std::int64_t ToInt() const {
        //add range checks
        auto ret=getAbsMantissa();
        if(exponent<0) {
            //needs range check
            ret>>=exponent;
        } else {
            ret<<=exponent;
        }
        return isNegative()?-ret:ret;
    }
    static Floating FromDouble(double x) {
        //FIXME handle nan, inf, zero
        int exp;
        double fraction=std::frexp (x, &exp );
        // |fraction| is in [0.5, 1), get it into mantissa with as many digits preserved as possible
        // if Rep is 64 bit, multiply with 2^63 and truncate towards zero
        const int shift=63;
        Rep mantissa=static_cast<Rep>(std::ldexp(fraction,shift));
        int exponent=exp-shift;
        return {mantissa,exponent};
    }
    double toDouble() {
        return std::ldexp(mantissa,exponent);
    }
    constexpr bool isNegative() const {return mantissa<0;}
    constexpr Rep getAbsMantissa() const { return isNegative()?-mantissa:mantissa;}
    int countLeadingZerosInMantissa() const {
        return notinstd::countl_zero(mantissa);
    }
    // floor(log2(abs(*this)))
    int fl2a() const {
        assert(!isZero());
        return 64-countLeadingZerosInMantissa()+exponent;
    }
    constexpr bool isZero() const {return mantissa==0;}
    // normalizes such that the exponent is as negative as possible,
    // meaning the bits in the mantissa are shuffled left as far as possible
    void normalize() {
        if(isZero()) { exponent=0; return;}
        //move all bits up as far as possible
        if(mantissa>0) {
            int nzeros=notinstd::countl_zero(mantissa);
            // shift so the first bit is zero
            if(nzeros>1) {
                mantissa<<=nzeros-1;
                exponent-=nzeros-1;
            }
        } else {
            //negative number. note, must special case intmin here to avoid ub.
            auto neg_mantissa=-mantissa;
            int nzeros=notinstd::countl_zero(neg_mantissa);
            // shift so the first bit is zero
            if(nzeros>1) {
                neg_mantissa<<=nzeros-1;
                exponent-=nzeros-1;
                mantissa=neg_mantissa;
            }
        }
    }
};
constexpr Floating operator+(Floating a, Floating b) {
    if(a.isZero()) return b;
    if(b.isZero()) return a;
return a;
}
/**
 * if rep is signed, this will work because any overflow has to be detected
 * by the compiler since it is prohibited in constexpr
 */
constexpr Floating operator*(Floating a, Floating b) {
    const int aisneg=a.isNegative();
    const int bisneg=b.isNegative();
    using Rep=Floating::Rep;
    Rep aabs=aisneg ? -a.mantissa : a.mantissa;
    Rep babs=bisneg ? -b.mantissa : b.mantissa;
    Rep ahi=aabs>>32;
    Rep alo=(aabs<<32)>>32;
    Rep bhi=babs>>32;
    Rep blo=(babs<<32)>>32;
    Rep res=ahi*bhi+ahi*alo+alo*bhi+alo*blo;
    const bool resultisneg= (aisneg ^ bisneg);
    return {resultisneg?-res:res,a.exponent+b.exponent};
}


#endif // FLOATINGPOINT_HPP
