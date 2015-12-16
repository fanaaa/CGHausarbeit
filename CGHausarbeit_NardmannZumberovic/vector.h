//
//  vector.h
//  Raytracer
//
//  Created by Fana Zumberovic on 11.03.15.
//  Copyright (c) 2015 Fana Zumberovic. All rights reserved.
//

#ifndef __Raytracer__vector__
#define __Raytracer__vector__

#include <stdio.h>

class Vector{
public:
    float X;
    float Y;
    float Z;
    
    Vector(float x, float y, float z);
    Vector();
    
    float dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector operator*(float c) const;
    Vector operator-() const;
    Vector& normalize();
    float length() const;
    float lengthSquared() const;
    Vector refelection(const Vector& normal) const;
    bool triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const;
};
#endif /* defined(__Raytracer__vector__) */
