#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_
namespace ps1
{
template <int FractionBits, typename StoreType, typename IntermediateType = long long, bool UseLibC = true>
struct FixedPoint
{
public:
    using Type = StoreType;
    FixedPoint() : _value(0) {}
    FixedPoint(const FixedPoint &other) : _value(other._value) {}
    FixedPoint(const int& value) : _value(value << FractionBits) {}
    FixedPoint(const volatile int& value) : _value(value << FractionBits) {}
    FixedPoint(const unsigned int& value) : _value(value << FractionBits) {}
    FixedPoint(const long& value) : _value(value << FractionBits) {}
    FixedPoint(const unsigned long& value) : _value(value << FractionBits) {}
    FixedPoint(const short& value) : _value(value << FractionBits) {}
    FixedPoint(const unsigned short& value) : _value(value << FractionBits) {}
    FixedPoint(const float& value) : _value(value * _ONE) {}
    FixedPoint(const double& value) : _value(value * _ONE) {}

    explicit operator int() const                      { return AsInt(); }
    explicit operator long() const                     { return AsInt(); }
    explicit operator unsigned int() const             { return AsInt(); }
    explicit operator unsigned long() const            { return AsInt(); }
    explicit operator float() const                    { return AsFloat(); }
    explicit operator double() const                   { return AsFloat(); }

    FixedPoint &operator=(const FixedPoint &other) { _value = other._value; return *this; }

    constexpr bool operator==(const FixedPoint &other) const { return _value == other._value; }
    constexpr bool operator!=(const FixedPoint &other) const { return _value != other._value; }
    constexpr bool operator<(const FixedPoint &other) const { return _value < other._value; }
    constexpr bool operator<=(const FixedPoint &other) const { return _value <= other._value; }
    constexpr bool operator>(const FixedPoint &other) const { return _value > other._value; }
    constexpr bool operator>=(const FixedPoint &other) const { return _value >= other._value; }

    constexpr FixedPoint operator~() const { return set(~_value); }
    constexpr FixedPoint operator+() const { return *this; }
    constexpr FixedPoint operator-() const { return set(-_value); }

    constexpr FixedPoint operator+(const FixedPoint &other) const { return set(_value + other._value); }
    constexpr FixedPoint operator-(const FixedPoint &other) const { return set(_value - other._value); }
    constexpr FixedPoint operator*(const FixedPoint &other) const { return set(((IntermediateType)_value * (IntermediateType)other._value) >> FractionBits); }
    inline constexpr FixedPoint operator/(const FixedPoint &other) const
    {
        if constexpr (UseLibC)
        {
            if(other._value == 0) return 0;
            return set((((IntermediateType)_value)<<FractionBits)/other._value);
        }
        else
        {
            if(other._value == 0) return 0;
            return set(Divide(_value,other._value));
        }
    }

    FixedPoint& operator+=(const FixedPoint &other) { _value += other._value; return *this; }
    FixedPoint& operator-=(const FixedPoint &other) { _value -= other._value; return *this; }
    FixedPoint& operator*=(const FixedPoint &other) { _value = ((IntermediateType)_value * (IntermediateType)other._value) >> FractionBits; return *this; }
    FixedPoint& operator/=(const FixedPoint &other) { _value = ((((IntermediateType) _value)<<FractionBits) / other._value); return *this; }
    FixedPoint& operator%=(const long& other) { _value = (((IntermediateType)_value)<<FractionBits) % other;  return *this; }

    FixedPoint& operator|=(const FixedPoint& other) { _value |=  other._value; return *this;}
    FixedPoint& operator&=(const FixedPoint& other) { _value &= other._value; return *this;}
    FixedPoint& operator^=(const FixedPoint& other) { _value ^= other._value; return *this;}

    FixedPoint operator<<(int numBits) const { return set(_value << numBits); }
    FixedPoint operator>>(int numBits) const { return set(_value >> numBits); }

    FixedPoint& operator<<=(int numBits) { _value <<= numBits; return *this;}
    FixedPoint& operator>>=(int numBits) { _value >>= numBits; return *this;}

    FixedPoint& operator++() { _value += 1<<FractionBits; return *this; } 
    FixedPoint& operator--() { _value -= 1<<FractionBits; return *this; }
    FixedPoint& operator++(int) { _value += 1<<FractionBits; return *this; } 
    FixedPoint& operator--(int) { _value -= 1<<FractionBits; return *this; }

    constexpr FixedPoint Abs() const { if(_value<0) return set(-_value); return set(_value); }

    constexpr StoreType AsFixedPoint() const { return _value; }
    static constexpr FixedPoint FromFixedPoint(StoreType value) { return set(value); }

    constexpr int AsInt() const { return _value >> FractionBits; }
    constexpr float AsFloat() const { return (float)_value / _ONE; }

    static constexpr FixedPoint PI() { return set(_ONE * 3.14159265359f); }
    static constexpr FixedPoint E() { return set(_ONE * 2.71828182845905); }
    static constexpr FixedPoint Zero = FixedPoint(0);
    static constexpr FixedPoint One = FixedPoint(1);
    static constexpr FixedPoint MinValue {._value = 2147483646};
    static constexpr FixedPoint MaxValue = {._value = 2147483647};
    static constexpr FixedPoint Epsilon() { return 0.000244140625; };

private:
    StoreType _value;
    static constexpr StoreType _ONE = 1 << FractionBits;
    static constexpr int INTEGER_BITS = sizeof(int) * 8 - FractionBits;
    static constexpr int FRACTION_MASK = (((int)0xFFFFFFFF) >> FractionBits);
    static constexpr int INTEGER_MASK = (-1 & ~FRACTION_MASK);
    static constexpr int FRACTION_RANGE = FRACTION_MASK + 1;
    static constexpr int MIN_INTEGER = (-2147483647 - 1) >> FractionBits;
    static constexpr int MAX_INTEGER = (2147483647) >> FractionBits;

    static constexpr FixedPoint set(StoreType value)
    {
        FixedPoint f;
        f._value = value;
        return f;
    }
    inline const unsigned int internalDivision(unsigned long long rem, unsigned int base) const
    {
        rem <<= FractionBits;
        unsigned long long b = base;
        unsigned long long res, d = 1;
        unsigned int high = rem >> 32;

        res = 0;
        if (high >= base) {
            high /= base;
            res = (unsigned long long)high << 32;
            rem -= (unsigned long long)(high * base) << 32;
        }

        while ((long long)b > 0 && b < rem) {
            b = b + b;
            d = d + d;
        }

        do {
            if (rem >= b) {
                rem -= b;
                res += d;
            }
            b >>= 1;
            d >>= 1;
        } while (d);

        return res;
    }

    inline constexpr int Divide(int a, int b) const
    {
        int s = 1;
        if (a < 0) {
            a = -a;
            s = -1;
        }
        if (b < 0) {
            b = -b;
            s = -s;
        }
        return internalDivision(a, b) * s;
    }

}; //struct FixedPoint

} //namespace ps1

#endif //_FIXED_POINT_H_