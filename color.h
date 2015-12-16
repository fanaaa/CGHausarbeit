//
//  color.h
//  CG_Praktikum1
//
//  Created by Benedikt Boss on 10.03.15.
//  Copyright (c) 2015 Benedikt Boss. All rights reserved.
//

#ifndef __CG_Praktikum1__color__
#define __CG_Praktikum1__color__

class Color {
public:
    float R;
    float G;
    float B;
    
    Color();
    Color(float, float, float);
    
    Color operator*(const Color&) const;
    Color operator*(const float) const;
    Color operator+(const Color&) const;
    Color& operator+=(const Color&);
};

#endif /* defined(__CG_Praktikum1__color__) */
