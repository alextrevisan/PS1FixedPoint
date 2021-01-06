#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_

template <int FRACTION_BITS, typename StoreType, typename IntermediateType = long long>
struct FixedPoint
{
private:
    static constexpr StoreType _ONE = 1 << FRACTION_BITS;

public:
    FixedPoint() : _value(0) {}
    FixedPoint(const FixedPoint &other) : _value(other._value) {}
    FixedPoint(const int value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const long value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const short value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const float value) : _value(value * _ONE) {}
    FixedPoint(const double value) : _value(value * _ONE) {}

    constexpr StoreType rawValue() const { return _value; }
    static constexpr FixedPoint fromRaw(StoreType value) { return set(value); }

    constexpr FixedPoint operator+(const FixedPoint &other) const { return set(_value + other._value); }

    constexpr FixedPoint operator-() const { return *this * -1; }
    constexpr FixedPoint operator-(const FixedPoint &other) const { return set(_value - other._value); }

    FixedPoint &operator=(const FixedPoint &other)
    {
        _value = other._value;
        return *this;
    }

    constexpr FixedPoint operator*(const FixedPoint &other) const { return set(((IntermediateType)_value * (IntermediateType)other._value) >> FRACTION_BITS); }

    constexpr FixedPoint operator/(const FixedPoint &other) const { return set((_value << FRACTION_BITS) / other._value); }

    constexpr bool operator==(const FixedPoint &other) const { return _value == other._value; }

    constexpr bool operator!=(const FixedPoint &other) const { return _value != other._value; }

    constexpr bool operator<(const FixedPoint &other) const { return _value < other._value; }

    constexpr bool operator<=(const FixedPoint &other) const { return _value <= other._value; }

    constexpr bool operator>(const FixedPoint &other) const { return _value > other._value; }

    constexpr bool operator>=(const FixedPoint &other) const { return _value >= other._value; }

    constexpr int toInt() const { return _value >> FRACTION_BITS; }

    constexpr float toFloat() const { return (float)_value / _ONE; }

    static constexpr FixedPoint PI() { return set(_ONE * 3.14159265359f); }

    static constexpr FixedPoint sqrt_approx(const FixedPoint& z)
    {
        union { float f; int i; } val {z.toFloat()};	/* Convert type, preserving bit pattern */
        /*
        * To justify the following code, prove that
        *
        * ((((val.i / 2^m) - b) / 2) + b) * 2^m = ((val.i - 2^m) / 2) + ((b + 1) / 2) * 2^m)
        *
        * where
        *
        * b = exponent bias
        * m = number of mantissa bits
        */
        val.i -= 1 << 23;	/* Subtract 2^m. */
        val.i >>= 1;		/* Divide by 2. */
        val.i += 1 << 29;	/* Add ((b + 1) / 2) * 2^m. */

        return FixedPoint(val.f);		/* Interpret again as float */
    }

    static constexpr FixedPoint sqrt(const FixedPoint &value)
    {
        IntermediateType num = IntermediateType{value._value} << FRACTION_BITS;
        IntermediateType res = 0;
        
        const IntermediateType hibit = ((find_highest_bit(value._value) + FRACTION_BITS) / 2 * 2);

        for (IntermediateType bit = IntermediateType{1} << hibit; bit != 0; bit >>= 2)
        {
            const IntermediateType val = res + bit;
            res >>= 1;
            if (num >= val)
            {
                num -= val;
                res += bit;
            }
        }
        // Round the last digit up if necessary
        res += (num > res);
        return set(static_cast<StoreType>(res));
    }

    static constexpr FixedPoint sqrtF2F (const FixedPoint& x )
    {
        unsigned int t, q, b, r;
        r = x._value;
        b = 0x40000000;
        q = 0;
        while( b > 0x40 )
        {
            t = q + b;
            if( r >= t )
            {
                r -= t;
                q = t + b; // equivalent to q += 2*b
            }
            r <<= 1;
            b >>= 1;
        }
        q >>= 10;
        return FixedPoint::fromRaw(q);
    }

    static constexpr FixedPoint sqrt2 (const FixedPoint& input )
    {
        int x = 0;
        int b = 0x40000000;
        int n = input._value;
        while( b > input._value)
            b <<= 2;

        while(b != 0)
        {
            if(n >= x + b)
            {
                n = n - x - b;
                x = x << 2 + b;
            }
            else
            {
                x <<= 1;
            }
            b <<=2;
        }
        return x>>=10;
    }
    // Returns the index of the most-signifcant set bit
    static inline long find_highest_bit(unsigned long long value) noexcept
    {
        return sizeof(value) * 8 - 1 - clz(value);
    }

    static int setBitNumber(int n) 
    { 
        if (n == 0) 
		return 0; 

        int msb = 0; 
        n >>= 1;
        while (n != 0) { 
            n >>= 1;
            msb++; 
        } 

        return msb;
    }

    static int clz(unsigned int x)
    {
        static constexpr char debruijn32[32] = 
        {
            0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19,
            1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18
        };
        x |= x>>1;
        x |= x>>2;
        x |= x>>4;
        x |= x>>8;
        x |= x>>16;
        x++;
        return 31 - debruijn32[x*0x076be629>>27];
    }

private:
    StoreType _value;
    static constexpr FixedPoint set(StoreType value)
    {
        FixedPoint f;
        f._value = value;
        return f;
    }
};

#endif //_FIXED_POINT_H_