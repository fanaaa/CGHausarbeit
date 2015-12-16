//
//  vector.cpp
//  Raytracer
//
//  Created by Fana Zumberovic on 11.03.15.
//  Copyright (c) 2015 Fana Zumberovic. All rights reserved.
//

#include "vector.h"
#include <cmath>

#define EPSILON 1e-6

Vector::Vector(float x, float y, float z){
    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector(){}

float Vector::dot(const Vector& v) const{
    return ((this->X * v.X) + (this->Y * v.Y) + (this->Z * v.Z));
}

Vector Vector::cross(const Vector& v) const{
    
 return Vector((this->Y * v.Z)-(this->Z * v.Y), (this->Z * v.X)-(this->X * v.Z), (this->X * v.Y)-(this->Y * v.X));
}

Vector Vector::operator+(const Vector& v) const{
    return Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}


Vector Vector::operator-(const Vector& v) const{
    return Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}

Vector& Vector::operator+=(const Vector& v){
    this->X = this->X + v.X;
    this->Y = this->Y + v.Y;
    this->Z = this->Z + v.Z;
    
    return *this;
}

Vector Vector::operator*(float c) const{
    return Vector(this->X * c, this->Y * c, this->Z * c);
}

Vector Vector::operator-() const{
    return Vector(-this->X, -this->Y, -this->Z);
}

Vector& Vector::normalize(){
    float length = this->length();
    if(length == 0){
        return *this;
    }
    this->X = this->X / length;
    this->Y = this->Y / length;
    this->Z = this->Z / length;
    
    return *this;
}

float Vector::length() const{
    return sqrtf(this->lengthSquared());
}

float Vector::lengthSquared() const{
    return (powf(this->X, 2) + powf(this->Y,2) + powf(this->Z,2));
}

Vector Vector::refelection(const Vector& normal) const{
    // formel: r = l - 2n(n.dot(l))
    
    float dot = this->dot(normal) * 2;
    Vector v = normal * dot;
    return *this - v;
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const{
    
    //bestimme Normalenvektor, der Ebene in der das Dreieck liegt
    
    Vector n = (b-a).cross(c-a);
    n.normalize();
    
    // berechne D = a*n (Distanz)
    //für s = (D - n*o)/(n*d)
    
    float dis = a.dot(n);
    float no = n.dot(*this);
    float nd = d.dot(n);
    
    s = ((dis - no) / nd);
    
    if(s < 0){
        return false;
    }
    
    //berechne p = o + sd
    
    Vector p = (*this)+(d*s);
    
    
    //Flaeche des Dreiecks berechnen A(abc) = length( (b-a) x (c-a)) / 2
    Vector vec = (b-a).cross(c-a);
    float length = vec.length();
    float flaecheAbc = length/2;
    
    
    // Flaeche des Dreickes A(acp) = length((c-a) x (p-a)) / 2
    vec = (c-a).cross(p-a);
    length = vec.length();
    float flaecheAcp = length/2;
    
    
    //Flaeche des Dreiecks A(bcp) = length((c-b) x (p-b))/2
    vec = (c-b).cross(p-b);
    length = vec.length();
    float flaecheBcp = length/2;
    
    //Flaeche des Dreiecks A(abp) = length((a-b) x (p-a))/2
    vec = (a-b).cross(p-a);
    length = vec.length();
    float flaecheAbp = length/2;
    
    
    float ergebnis = flaecheAbc - (flaecheAbp + flaecheAcp + flaecheBcp);
    
    return  (ergebnis < EPSILON && ergebnis > -EPSILON);
    
}


