//
//  BoundingBox.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Philipp Nardmann on 18.01.16.
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

#include <stdio.h>
#include "BoundingBox.h"


BoundingBox::~BoundingBox(){
}

BoundingBox::BoundingBox()
{
    
}

BoundingBox::BoundingBox( const Vector& min, const Vector& max)
{
    this->Min = min;
    this->Max = max;
}