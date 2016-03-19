//
//  color.cpp
//  CG_Praktikum1
//
//  Created by Fana Zumberovic on 11.03.15.
//  Copyright (c) 2015  Fana Zumberovic. All rights reserved.
//

#include "color.h"

Color::Color() {
    R = 0.f;
    G = 0.f;
    B = 0.f;
}

Color::Color(float r, float g, float b) {
    R = r;
    G = g;
    B = b;
}

Color Color::operator*(const Color& c) const {
    return Color(R*c.R, G*c.G, B*c.B);
}

Color Color::operator*(const float Factor) const {
    return Color(R*Factor, G*Factor, B*Factor);
}

Color Color::operator+(const Color& c) const {
    return Color(R+c.R, G+c.G, B+c.B);
}

Color& Color::operator+=(const Color& c) {
    R += c.R;
    G += c.G;
    B += c.B;
    return *this;
}