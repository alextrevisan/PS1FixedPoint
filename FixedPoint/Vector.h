#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <psxgte.h>
namespace ps1
{
template<typename VectorType>
struct Vector3F;

typedef  Vector3F<VECTOR> Vector3D;
typedef Vector3F<SVECTOR> SVector3D;

template<typename VectorType>
struct Vector3F : public VectorType
{
    using type = decltype(VectorType::vx);
    Vector3F()
    :VectorType{0,0,0}
    {}
    Vector3F(type vx, type vy, type vz)
    :VectorType{vx, vy, vz}
    {

    }

    Vector3F(const VectorType& vector )
    :VectorType::vx(vector.vx), VectorType::vy(vector.vy), VectorType::vz(vector.vz)
    {

    }
    const Vector3F operator-(const Vector3F& other) const
    {
        return {VectorType::vx - other.vx, VectorType::vy - other.vy, VectorType::vz - other.vz};
    }

    inline static constexpr Vector3D crossProduct(const Vector3F& right)
    {
        return {((VectorType::vy*right.vz)-(VectorType::vz*right.vy)>>12),
                ((VectorType::vz*right.vx)-(VectorType::vx*right.vz)>>12),
                ((VectorType::vx*right.vy)-(VectorType::vy*right.vx)>>12)
        };
    }

    inline static constexpr Vector3D crossProduct(const Vector3F& left, const Vector3F& right)
    {
        return{(((left.vy*right.vz)-(left.vz*right.vy))>>12),
                (((left.vz*right.vx)-(left.vx*right.vz))>>12),
                (((left.vx*right.vy)-(left.vy*right.vx))>>12)
        };
    }

    constexpr inline int length() const
    {
        return SquareRoot12(dotProduct());
    }

    static const int length(const Vector3F& vector)
    {
        return SquareRoot12(dotProduct(vector, vector));
    }

    const inline int dotProduct() const
    {
        return VectorType::vx*VectorType::vx + VectorType::vy*VectorType::vy + VectorType::vz*VectorType::vz;
    }

    const int dotProduct(const Vector3F& other) const
    {
        return VectorType::vx*other.vx + VectorType::vy*other.vy + VectorType::vz * other.vz;
    }

    const inline SVector3D normalise()
    {
        SVector3D output;
        VectorNormalS(this, &output);
        return output;
    }

    operator VectorType& ()
    {
        return this;
    }
};

const Vector3D operator*(const Vector3D& vector, const int other) 
{
    return {vector.vx*other,vector.vy*other,vector.vz*other};
}

const Vector3D operator*(const SVector3D& vector, const int other) 
{
    return {vector.vx*other,vector.vy*other,vector.vz*other};
}


const Vector3D operator+(const Vector3D& left, const auto& right) 
{
    return {left.vx+right.vx,left.vy+right.vy,left.vz+right.vz};
}

const Vector3D operator+(const auto& left, const Vector3D& right) 
{
    return {left.vx+right.vx,left.vy+right.vy,left.vz+right.vz};
}

} //namespace ps1

#endif //#ifndef _VECTOR_H_