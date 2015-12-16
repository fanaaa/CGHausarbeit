//
//  Object.hpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Collaborator: Philipp Nardmann
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include <stdio.h>
#include "vector.h"
#include "Object.h"
class Object{
public:
    Vector translation;
    Vector scaling;
    Vector rotationVector;
    float rotationAngle;
    char name;
    
    ~Object();
    Object(Vector translation,Vector scaling,Vector rotationVector,float rotationAngle);
    Object();
};
#endif /* Object_h */
