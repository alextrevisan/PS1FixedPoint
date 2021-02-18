#ifndef _VECTOR_H_
#define _VECTOR_H_

template<typename FloatType, typename ConvertType>
struct _VECTOR
{
    typedef FloatType _Float;

    _VECTOR(const _Float& x, const _Float y, const _Float z):vx(x), vy(y), vz(z){}
    _VECTOR(const SVECTOR& input):vx(_Float::FromFixedPoint(input.vx)), vy(_Float::FromFixedPoint(input.vy)), vz(_Float::FromFixedPoint(input.vz)){}
    _VECTOR(const VECTOR& input):vx(_Float::FromFixedPoint(input.vx)), vy(_Float::FromFixedPoint(input.vy)), vz(_Float::FromFixedPoint(input.vz)){}
    _VECTOR():vx(0), vy(0), vz(0){}

    const _VECTOR operator-(const _VECTOR& other) const
    {
        return {vx - other.vx, vy - other.vy, vz - other.vz};
    }

    inline const _VECTOR crossProduct(const _VECTOR& right) const
    {
        return {((vy*right.vz)-(vz*right.vy)),
                ((vz*right.vx)-(vx*right.vz)),
                ((vx*right.vy)-(vy*right.vx))
        };
    }

    static const _VECTOR crossProduct(const _VECTOR& left, const _VECTOR& right)
    {
        return {((left.vy*right.vz)-(left.vz*right.vy)),
                ((left.vz*right.vx)-(left.vx*right.vz)),
                ((left.vx*right.vy)-(left.vy*right.vx))
        };
    }

    const _VECTOR normalize() const
    {
        /*const VECTOR input = {vx.toInt(), vy.toInt(), vz.toInt()};
        SVECTOR output;
        VectorNormalS(&input, &output);
        return output;*/
        const _Float l = length();
        return { vx / l, vy / l, vz / l };
    }

    static const _VECTOR normalize(const _VECTOR& vector)
    {
        /*const VECTOR input = {vector.vx.AsFixedPoint(), vector.vy.AsFixedPoint(), vector.vz.AsFixedPoint()};
        SVECTOR output;
        VectorNormalS(&input, &output);
        return output;*/
        const _Float l = length(vector);
        return { vector.vx / l, vector.vy / l, vector.vz / l };
    }

    constexpr _Float length() const
    {
        return _Float::FromFixedPoint(SquareRoot12(dotProduct().AsFixedPoint()));
    }

    static const _Float length(const _VECTOR& vector)
    {
        return _Float::FromFixedPoint(SquareRoot12(dotProduct(vector, vector).AsFixedPoint()));
    }

    const _Float dotProduct() const
    {
        return vx*vx + vy*vy + vz*vz;
    }

    const _Float dotProduct(const _VECTOR& other) const
    {
        return vx*other.vx + vy*other.vy + vz * other.vz;
    }

    static const _Float dotProduct(const _VECTOR& left, const _VECTOR& right)
    {
        return left.vx*right.vx + left.vy*right.vy + left.vz * right.vz;
    }

    const _VECTOR operator*(const _Float& other) const 
    {
        return {vx*other,vy*other,vz*other};
    }

    const _VECTOR operator+(const _VECTOR& other) const 
    {
        return {vx+other.vx,vy+other.vy,vz+other.vz};
    }

    operator ConvertType()
    {
        return ConvertType{vx.AsFixedPoint(),vy.AsFixedPoint(),vz.AsFixedPoint()};
    }

    operator ConvertType*()
    {
        return (ConvertType*) this;
    }
    
    _Float vx, vy, vz;
};

typedef _VECTOR<FixedPoint<12, int, long long>, VECTOR> Vector3D;
typedef _VECTOR<FixedPoint<12, short, long long>, SVECTOR> SVector3D;
#endif //#ifndef _VECTOR_H_