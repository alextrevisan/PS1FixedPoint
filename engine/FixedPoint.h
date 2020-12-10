#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_

template<int FIXED_BITS>
class FixedPoint
{
public:
    FixedPoint():_value(0){}

    FixedPoint(int value):_value(value << FIXED_BITS){};

    FixedPoint(float value):_value(value * one()._value){}

    int rawValue() const
    {
        return _value;
    }

    FixedPoint operator+(const FixedPoint& other) const
    {
        return set(_value + other._value);
    }

    FixedPoint operator-(const FixedPoint& other) const
    {
        return set(_value - other._value);
    }

    FixedPoint operator*(const FixedPoint& other) const
    {
        return set(((long)_value * (long)other._value) >> FIXED_BITS);
    }

    FixedPoint operator/(const FixedPoint& other) const
    {
        return set((_value << FIXED_BITS) / other._value);
    }

    constexpr bool operator==(const FixedPoint& other)
    {
        return _value == other._value;
    }

    constexpr bool operator==(const int other)
    {
        return _value == other << FIXED_BITS;
    }

    constexpr bool operator==(float other)
    {
        return _value == (1 << FIXED_BITS) * other;
    }

    constexpr bool operator<(const float other)
    {
        return _value < (other * one()._value);
    }

    int toInt() const 
    {
        return _value >> FIXED_BITS;
    }

    float toFloat() const 
    {
        return (float)_value / one()._value;
    }

    static constexpr FixedPoint one()
    {
        return set(1 << FIXED_BITS);
    };

    static constexpr FixedPoint PI()
    {
        return set((1 << FIXED_BITS) * 3.14159265359f);
    }

private:
    int _value;
    static constexpr FixedPoint set(int value)
    {
        FixedPoint f;
        f._value = value;
        return f;
    }
};

#endif //_FIXED_POINT_H_