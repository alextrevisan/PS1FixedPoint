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
    FixedPoint(const int& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const volatile int& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const unsigned int& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const long& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const unsigned long& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const short& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const unsigned short& value) : _value(value << FRACTION_BITS) {}
    FixedPoint(const float& value) : _value(value * _ONE) {}
    FixedPoint(const double& value) : _value(value * _ONE) {}

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
    constexpr FixedPoint operator*(const FixedPoint &other) const { return set(((IntermediateType)_value * (IntermediateType)other._value) >> FRACTION_BITS); }
    constexpr FixedPoint operator/(const FixedPoint &other) const { return set((_value << FRACTION_BITS) / other._value); }

    FixedPoint& operator+=(const FixedPoint &other) { _value += other._value; return *this; }
    FixedPoint& operator-=(const FixedPoint &other) { _value -= other._value; return *this; }
    FixedPoint& operator*=(const FixedPoint &other) { _value = ((IntermediateType)_value * (IntermediateType)other._value) >> FRACTION_BITS; return *this; }
    FixedPoint& operator/=(const FixedPoint &other) { _value = ((((IntermediateType) _value)<<FRACTION_BITS) / other._value); return *this; }
    FixedPoint& operator%=(const long& other) { _value = (((IntermediateType)_value)<<FRACTION_BITS) % other;  return *this; }

    FixedPoint& operator|=(const FixedPoint& other) { _value |=  other._value; return *this;}
	FixedPoint& operator&=(const FixedPoint& other) { _value &= other._value; return *this;}
	FixedPoint& operator^=(const FixedPoint& other) { _value ^= other._value; return *this;}

    FixedPoint operator<<(int numBits) const { return set(_value << numBits); }
	FixedPoint operator>>(int numBits) const { return set(_value >> numBits); }

    FixedPoint& operator<<=(int numBits) { _value <<= numBits; return *this;}
	FixedPoint& operator>>=(int numBits) { _value >>= numBits; return *this;}

    FixedPoint& operator++() { _value += 1<<FRACTION_BITS; return *this; } 
	FixedPoint& operator--() { _value -= 1<<FRACTION_BITS; return *this; }
    FixedPoint& operator++(int) { _value += 1<<FRACTION_BITS; return *this; } 
	FixedPoint& operator--(int) { _value -= 1<<FRACTION_BITS; return *this; }

    constexpr FixedPoint Abs() const { if(_value<0) return set(-_value); return set(_value); }

    constexpr StoreType AsFixedPoint() const { return _value; }
    static constexpr FixedPoint FromFixedPoint(StoreType value) { return set(value); }


    FixedPoint &operator=(const FixedPoint &other)
    {
        _value = other._value;
        return *this;
    }

    constexpr int AsInt() const { return _value >> FRACTION_BITS; }
    constexpr float AsFloat() const { return (float)_value / _ONE; }

    static constexpr FixedPoint PI() { return set(_ONE * 3.14159265359f); }

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