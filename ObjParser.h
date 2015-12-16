//
//  ObjParser.h
//  Praktikum3
//
//  Created by Boss, Mark on 15.04.15.
//  Copyright (c) 2015 Boss, Mark. All rights reserved.
//

#ifndef __Praktikum3__ObjParser__
#define __Praktikum3__ObjParser__

#include <stdio.h>
#include "Model.h"
#include <vector>
#include <map>
#include "vector.h"
using namespace std;

class ObjParser
{
public:
    ObjParser();
    vector<Vertex> parseObjFile(const char* Filename, bool fit);
    BoundingBox getBoundingBox();
    map<string, Material> getMtlMap();
private:
    vector<Vector> tempPositionVec;
    vector<Texcoord> tempTexCoordVec;
    map<string, Material> mtlMap;
    BoundingBox bounds;
    vector<Vertex> vertices;
    void findBounds();
    void trim(string& s);
    vector<string> split(const string& s, char seperator);
};

#endif /* defined(__Praktikum3__ObjParser__) */
