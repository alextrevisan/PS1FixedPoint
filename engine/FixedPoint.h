#ifndef _FIXED_POINT_H_
#define _FIXED_POINT_H_

template<int FIXED_BITS>
class FixedPoint
{
public:
    ~FixedPoint(){};
    static constexpr FixedPoint fromInt(int value)
    {
        return value << FIXED_BITS;
    }

    static constexpr FixedPoint fromFloat(float value)
    {
        return value * one()._value;
    }

    int rawValue() const
    {
        return _value;
    }

    FixedPoint operator+(const FixedPoint& other) const
    {
        return _value + other._value;
    }

    FixedPoint operator-(const FixedPoint& other) const
    {
        return _value - other._value;
    }

    FixedPoint operator*(const FixedPoint& other) const
    {
        return ((long)_value * (long)other._value) >> FIXED_BITS;
    }

    FixedPoint operator/(const FixedPoint& other) const
    {
        return (_value << FIXED_BITS) / other._value;
    }

    int toInt() const 
    {
        return _value >> FIXED_BITS;
    }

    int toFloat() const 
    {
        return _value / one()._value;
    }

    static constexpr FixedPoint one()
    {
        return 1 << FIXED_BITS;
    } 

private:
    int _value;
    FixedPoint(int value)
    :_value(value){};
};

#endif //_FIXED_POINT_H_