#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <psxgte.h>
#include <inline_c.h>

namespace ps1
{
template<typename VectorType>
struct Vector3F;

typedef  Vector3F<VECTOR> Vector3D;
typedef Vector3F<SVECTOR> SVector3D;

inline void gte_ldv0_f(const VECTOR& v0)
{
    int xy11 = ((v0.vx&0xFFFF)|(v0.vy<<16));
    int z11 = v0.vz;
    asm volatile ("mtc2 %0, $0\n" : "+r"(xy11) : : );
    asm volatile ("mtc2 %0, $1\nnop\nnop\n" : "+r"(z11) : : );   
}

#define gte_stlvnl( r0 ) __asm__ volatile (	\
	"swc2	$25, 0( %0 );"	\
	"swc2	$26, 4( %0 );"	\
	"swc2	$27, 8( %0 )"	\
	:						\
	: "r"( r0 )	\
	: "memory" )

#define gte_ldlvl( r0 ) __asm__ volatile ( \
	"lwc2	$9 , 0( %0 );"	\
	"lwc2	$10 , 4( %0 );"	\
    "lwc2	$11 , 8( %0 );"	\
	:						\
	: "r"( r0 )				\
	: "$t0" )

inline void gte_ldlvl_f(const VECTOR& v0)
{
    asm volatile ("mtc2 %0, $25\n" :  : "r"(v0.vx): );
    asm volatile ("mtc2 %0, $26\n" :  : "r"(v0.vy): );
    asm volatile ("mtc2 %0, $27\n" :  : "r"(v0.vz): );
}

#define gte_ldlvl_1( r0 ) __asm__ volatile (			\
	"mtc2	%0, $9"					\
	:							\
	: "r"( r0 ) )

#define gte_ldlvl_2( r0 ) __asm__ volatile (			\
	"mtc2	%0, $10"					\
	:							\
	: "r"( r0 ) )
#define gte_ldlvl_3( r0 ) __asm__ volatile (			\
	"mtc2	%0, $11"					\
	:							\
	: "r"( r0 ) )
#define gte_stmac_1() ({ u_long __value;\
	__asm__ volatile (			\
	".set noreorder;"			\
	"mfc2	%0, $25;"					\
	"nop;"					\
	".set reorder"				\
	: "=r"( __value ) :);\
	__value;})
#define gte_stmac_2() ({ u_long __value;\
	__asm__ volatile (			\
	".set noreorder;"			\
	"mfc2	%0, $26;"					\
	"nop;"					\
	".set reorder"				\
	: "=r"( __value ) :);\
	__value;})
#define gte_stmac_3() ({ u_long __value;\
	__asm__ volatile (			\
	".set noreorder;"			\
	"mfc2	%0, $27;"					\
	"nop;"					\
	".set reorder"				\
	: "=r"( __value ) :);\
	__value;})
inline void gte_stlvnl_f(VECTOR& v0)
{
    asm volatile ("mfc2 %0, $9\n" :   "=r"(v0.vx): :);
    asm volatile ("mfc2 %0, $10\n" :   "=r"(v0.vy): :);
    asm volatile ("mfc2 %0, $11\n" :   "=r"(v0.vz): :);
}

template<typename VectorType>
struct Vector3F : public VectorType
{
    using type = decltype(VectorType::vx);
    Vector3F()
    :VectorType{0,0,0}
    {}
    Vector3F(type vx, type vy, type vz)
    :VectorType{.vx=vx, .vy=vy, .vz=vz}
    {

    }

    Vector3F(const VectorType& vector )
    :VectorType{vector}
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
        const auto dot = dotProduct();
        return SquareRoot12(dot<<12);
    }

    static const int length(const Vector3F& vector)
    {
        return SquareRoot12(dotProduct(vector, vector));
    }

    constexpr inline int dotProduct() const
    {
        if constexpr(is_same<VectorType, SVECTOR>::value)
        {

            VECTOR myinput{VectorType::vx<<12, VectorType::vy<<12, VectorType::vz<<12};
            gte_ldlvl(&myinput);
            gte_sqr12();
            VECTOR out;
            gte_stlvnl(&out);
            return (out.vx+out.vy+out.vz)>>12;
        }
        else
        {
            return VectorType::vx*VectorType::vx + VectorType::vy*VectorType::vy + VectorType::vz*VectorType::vz;        
        }
    }

    const int dotProduct(const Vector3F& other) const
    {
        return VectorType::vx*other.vx + VectorType::vy*other.vy + VectorType::vz * other.vz;
    }

    const inline SVector3D normalize()
    {
        SVector3D output;
        VectorNormalS(this, &output);
        return output;
    }

    static constexpr inline SVector3D normalize(const VectorType& input)
    {
        SVector3D output;
        VectorNormalS(&input, &output);
        return output;
    }

    operator VectorType& ()
    {
        return this;
    }

    inline constexpr operator VectorType*() const
    {
        return this;
    }

    inline operator VectorType*()
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