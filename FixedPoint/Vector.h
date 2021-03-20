#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <psxgte.h>
namespace ps1
{
template<typename FloatType>
struct Vector3F
{
    typedef FloatType Float;

    Vector3F(const Float& x, const Float y, const Float z):vx(x), vy(y), vz(z){}
    Vector3F(const SVECTOR& input):vx(Float::FromFixedPoint(input.vx)), vy(Float::FromFixedPoint(input.vy)), vz(Float::FromFixedPoint(input.vz)){}
    Vector3F(const VECTOR& input):vx(Float::FromFixedPoint(input.vx)), vy(Float::FromFixedPoint(input.vy)), vz(Float::FromFixedPoint(input.vz)){}
    Vector3F():vx(0), vy(0), vz(0){}

    const Vector3F operator-(const Vector3F& other) const
    {
        return {vx - other.vx, vy - other.vy, vz - other.vz};
    }

    inline const Vector3F crossProduct(const Vector3F& right) const
    {
        return {((vy*right.vz)-(vz*right.vy)),
                ((vz*right.vx)-(vx*right.vz)),
                ((vx*right.vy)-(vy*right.vx))
        };
    }

    static const Vector3F crossProduct(const Vector3F& left, const Vector3F& right)
    {
        return {((left.vy*right.vz)-(left.vz*right.vy)),
                ((left.vz*right.vx)-(left.vx*right.vz)),
                ((left.vx*right.vy)-(left.vy*right.vx))
        };
    }

    const Vector3F normalize() const
    {
        const VECTOR input = {vx.AsInt(), vy.AsInt(), vz.AsInt()};
        SVECTOR output;
        VectorNormalS(&input, &output);
        return output;
        //const Float l = length();
        //return { vx / l, vy / l, vz / l };
    }

    static inline const Vector3F normalize(const Vector3F& vector)
    {
        SVECTOR output;
        VectorNormalS(vector, &output);
        return output;
        
        /*const Float l = length(vector);
        return { vector.vx / l, vector.vy / l, vector.vz / l };*/
    }

    constexpr inline Float length() const
    {
        return Float::FromFixedPoint(SquareRoot12(dotProduct().AsFixedPoint()));
    }

    static const Float length(const Vector3F& vector)
    {
        return Float::FromFixedPoint(SquareRoot12(dotProduct(vector, vector).AsFixedPoint()));
    }

    const Float dotProduct() const
    {
        return vx*vx + vy*vy + vz*vz;
    }

    const Float dotProduct(const Vector3F& other) const
    {
        return vx*other.vx + vy*other.vy + vz * other.vz;
    }

    static const Float dotProduct(const Vector3F& left, const Vector3F& right)
    {
        return left.vx*right.vx + left.vy*right.vy + left.vz * right.vz;
    }

    const Vector3F operator*(const Float& other) const 
    {
        return {vx*other,vy*other,vz*other};
    }

    const Vector3F operator+(const Vector3F& other) const 
    {
        return {vx+other.vx,vy+other.vy,vz+other.vz};
    }

    inline constexpr operator SVECTOR()
    {
        return SVECTOR{vx.AsFixedPoint(),vy.AsFixedPoint(),vz.AsFixedPoint(),0};
    }

    inline constexpr operator SVECTOR*()
    {
        return (SVECTOR*) this;
    }

    inline constexpr operator VECTOR*() const 
    {
        return (VECTOR*) this;
    }

    inline constexpr operator const VECTOR() const 
    {
        return VECTOR{vx.AsFixedPoint(),vy.AsFixedPoint(),vz.AsFixedPoint()};
    }

    static inline const Vector3F FromSVECTOR(const SVECTOR& other)
    {
        return Vector3F{
            Float::FromFixedPoint(other.vx),
            Float::FromFixedPoint(other.vy),
            Float::FromFixedPoint(other.vz)
        };
    }

    static inline const Vector3F FromFixedPoint(const typename FloatType::Type vx, const typename FloatType::Type vy, const typename FloatType::Type vz)
    {
        return Vector3F{
            Float::FromFixedPoint(vx),
            Float::FromFixedPoint(vy),
            Float::FromFixedPoint(vz)
        }; 
    }

    
    Float vx, vy, vz;
};

} //namespace ps1

#endif //#ifndef _VECTOR_H_