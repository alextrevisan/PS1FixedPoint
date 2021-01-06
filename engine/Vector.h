#ifndef _VECTOR_H_
#define _VECTOR_H_

template<int FIXED_BITS, typename storeType, typename castType = long>
struct _VECTOR
{
	typedef FixedPoint<FIXED_BITS, storeType, castType> _Float;

	_VECTOR(const _Float& x, const _Float y, const _Float z):vx(x), vy(y), vz(z){}
    _VECTOR(const SVECTOR& input):vx(_Float::fromRaw(input.vx)), vy(_Float::fromRaw(input.vy)), vz(_Float::fromRaw(input.vz)){}
	_VECTOR(const VECTOR& input):vx(_Float::fromRaw(input.vx)), vy(_Float::fromRaw(input.vy)), vz(_Float::fromRaw(input.vz)){}
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
        /*const VECTOR input = {vector.vx.rawValue(), vector.vy.rawValue(), vector.vz.rawValue()};
        SVECTOR output;
        VectorNormalS(&input, &output);
        return output;*/
        const _Float l = length(vector);
		return { vector.vx / l, vector.vy / l, vector.vz / l };
    }

    constexpr _Float length() const
    {
        //return _Float::sqrt_approx(dotProduct());

        //SquareRoot12 is 17x faster but precision is bad
        return _Float::fromRaw(SquareRoot12(dotProduct().rawValue()));
    }

    static const _Float length(const _VECTOR& vector)
    {
        return _Float::sqrt(dotProduct(vector, vector));
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

	_Float vx, vy, vz;
};

typedef _VECTOR<12, short, int> SVector3D;
typedef _VECTOR<12, int, long long> Vector3D;
#endif //#ifndef _VECTOR_H_