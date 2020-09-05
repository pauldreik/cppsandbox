#ifndef FLOATINGPOINT_HPP
#define FLOATINGPOINT_HPP

#include <cstdint>
#include <cmath>
#include <cassert>
#include <type_traits>
#include <optional>

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
int countr_zero(std::int64_t x) {
    if(x==0) {
        return 64;
    }
    if(x>0) {
        if constexpr(std::is_same_v<std::int64_t,long>)
        return __builtin_ctzl(x);
        if constexpr(std::is_same_v<std::int64_t,long long>)
        return __builtin_ctzll(x);
    }
    // watch out for overflow here
    return countr_zero(-x);
}
}

struct Floating {
    using Rep=std::int64_t;
    // INT_MIN is forbidden
    Rep mantissa;
    int exponent;
    void print() const {
        std::cout<<mantissa<<'*'<<"2^"<<exponent<<'\n';
    }
    constexpr static Floating FromMantissaAndExp(std::int64_t mantissa,int exp) {
         if(mantissa==std::numeric_limits<std::int64_t>::min())
             return {mantissa/2,exp+1,NoNormalizeNeeded{}};
             else
        return Floating{mantissa,exp};
    }
    constexpr static Floating FromInt(std::int64_t x) {
        if(x==std::numeric_limits<std::int64_t>::min())
            return {x/2,1,NoNormalizeNeeded{}};
        else
            return Floating{x,0};
    }
private:
    struct NoNormalizeNeeded{};
    constexpr Floating(Rep mant, int exp) : mantissa(mant),exponent(exp) {
        normalize();
        EnsureInvariantHolds();
    }
    constexpr Floating(Rep mant, int exp,NoNormalizeNeeded) : mantissa(mant),exponent(exp) {
        EnsureInvariantHolds();
    }
public:
    constexpr Floating operator-() const {
        return FromMantissaAndExp(-mantissa,exponent);
    }
    //converts to int, if it can be done without loss
    constexpr std::optional<std::int64_t> ToInt() const {
        if(isZero())
            return {std::int64_t{0}};
        auto ret=getAbsMantissa();

        if(exponent<-62) {
            // no way this could work
            return {};
        }

        if(exponent<0) {
            //for this to be lossless, there must be -exponent trailing zeros
            //or more
            const int n_trailing=notinstd::countr_zero(ret);
            if( n_trailing< -exponent) {
                //nope, not enough
                return {};
            }
            ret>>=-exponent;
            return isNegative()?-ret:ret;
        }
        if(exponent==0)
            return isNegative()?-ret:ret;

        //only chance to get a valid positive number is
        //to have intmin which is stored as "(intmin/2)*2^1"
        if(exponent==1 && mantissa==std::numeric_limits<std::int64_t>::min()/2) {
            return std::numeric_limits<std::int64_t>::min();
        }
        //positive exponent means it is too big, because
        //the stored number is always either all zeros (checked above)
        //or the second most significant bit is set due to normalization.
        return {};
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
    constexpr bool isPositive() const {return mantissa>0;}
    constexpr int sign() const {if (mantissa<0) return -1;
        if (mantissa>0) return 1;
        return 0;
                               }
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
    Floating& normalize() {
        if(isZero()) { exponent=0; return *this;}
        bool ispositive=mantissa>0;
        //move all bits up as far as possible
        if(ispositive) {
            int nzeros=notinstd::countl_zero(mantissa);
            // shift so the first bit is zero
            if(nzeros>1) {
                mantissa<<=nzeros-1;
                exponent-=nzeros-1;
            }
        } else {
            //negative number.
            if(mantissa==std::numeric_limits<std::int64_t>::min()) {
                mantissa/=2;
                exponent+=1;
                return *this;
            }
            auto neg_mantissa=-mantissa;
            int nzeros=notinstd::countl_zero(neg_mantissa);
            // shift so the first bit is zero
            if(nzeros>1) {
                neg_mantissa<<=nzeros-1;
                exponent-=nzeros-1;
                mantissa=-neg_mantissa;
            }
        }
        return *this;
    }
    void EnsureInvariantHolds()const {
        // if zero, must be *all* zero
        if(mantissa==0) {
            assert(exponent==0);
            return;
        }
        //forbidden value for nonzero
        assert(mantissa!=std::numeric_limits<std::int64_t>::min());
        //must not be subnormal=>has a one as second most significant bit
        auto absmantissa=getAbsMantissa();
        assert(absmantissa>>62==0b01L);
    }
    friend constexpr Floating operator+(const Floating& a, const Floating& b);
    friend constexpr Floating operator*(Floating a, Floating b);
    friend bool operator==(const Floating& a, const Floating& b) {
        return a.mantissa==b.mantissa && a.exponent==b.exponent;
    }
};

constexpr Floating operator+(const Floating& a, const Floating& b) {
    if(a.isZero()) return b;
    if(b.isZero()) return a;

    //which one is biggest?
    const int asize=a.fl2a();
    const int bsize=b.fl2a();

    if(asize>=bsize) {
        //downshift b
        int nshift=asize-bsize;
        if (nshift>62) {
            //b is too small to be noticed
            return a;
        }
        auto toadd=b.getAbsMantissa()>>nshift;
        if(b.isNegative()) {
            toadd=-toadd;
        }
        //play it safe
        Floating::Rep ret=0;
        if(__builtin_add_overflow(a.mantissa,toadd,&ret)) {
            //overflow. scale one bit away.
            auto lastbit=(a.sign()*(a.mantissa &0b1)+b.sign()*(toadd&0b1))/2;
            return Floating::FromMantissaAndExp(a.mantissa/2 + toadd/2 + lastbit, a.exponent+1);
        }
        return Floating::FromMantissaAndExp(ret,a.exponent);
    }

    //lazy. flip the order.
    return b+a;
}
/**
 * if rep is signed, this will work because any overflow has to be detected
 * by the compiler since it is prohibited in constexpr
 */
constexpr Floating operator*(Floating a, Floating b) {
    if(a.isZero() || b.isZero())
        return Floating{0,0};
    const int aisneg=a.isNegative();
    const int bisneg=b.isNegative();
    using Rep=Floating::Rep;
    using URep=std::make_unsigned_t<Rep>;
    URep aabs=aisneg ? -a.mantissa : a.mantissa;
    URep babs=bisneg ? -b.mantissa : b.mantissa;
    Rep ahi=aabs>>32;
    Rep alo=(aabs<<32)>>32;

    Rep bhi=babs>>32;
    Rep blo=(babs<<32)>>32;

    // add in the order of small to large
    const int sumexp=a.exponent+b.exponent;
    Floating hihi{ahi*bhi,sumexp+64};
    Floating hilo{ahi*blo,sumexp+32};
    Floating lohi{alo*bhi,sumexp+32};
    Floating lolo{alo*blo,sumexp};
    Floating sum = hihi+(hilo+(lohi+lolo));
    const bool resultisneg= (aisneg ^ bisneg);
    return resultisneg?-sum:sum;
}


#endif // FLOATINGPOINT_HPP
