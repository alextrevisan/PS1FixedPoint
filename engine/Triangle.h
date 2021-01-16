#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include <Vector.h>

struct Triangle
{
    Vector3D vertex0, vertex1, vertex2;
};

inline const bool RayIntersectsTriangle(const Vector3D& origin, 
                           const Vector3D& direction, 
                           const Triangle& triangle,
                           Vector3D& intersectPoint)
{
    typedef Vector3D::_Float Float;
    const Float EPSILON = 0.01f;
    const Vector3D& vertex0 = triangle.vertex0;
    const Vector3D& vertex1 = triangle.vertex1;  
    const Vector3D& vertex2 = triangle.vertex2;
    const Vector3D& edge1 = vertex1 - vertex0;
    const Vector3D& edge2 = vertex2 - vertex0;
    const Vector3D& h = direction.crossProduct(direction, edge2);
    
    const Float& a = edge1.dotProduct(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    const Float& f = Float(1.0f)/a;
    const Vector3D& s = origin - vertex0;
    const Float& u = s.dotProduct(s, h) * f;
    if (u < 0.0 || u > 1.0)
        return false;
    const Vector3D& q = s.crossProduct(s, edge1);
    const Float& v = f * direction.dotProduct(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    const Float& t = f * edge2.dotProduct(q);
    if (t > EPSILON) // ray intersection
    {
        intersectPoint = origin + direction * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

#endif //#ifndef _TRIANGLE_H_