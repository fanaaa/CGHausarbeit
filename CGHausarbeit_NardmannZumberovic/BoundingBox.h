//
//  BoundingBox.h
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Philipp Nardmann on 18.01.16.
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

#ifndef BoundingBox_h
#define BoundingBox_h

#include "vector.h"

class BoundingBox
{
public:
    ~BoundingBox();
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    Vector Min;
    Vector Max;
};

#endif /* BoundingBox_h */
