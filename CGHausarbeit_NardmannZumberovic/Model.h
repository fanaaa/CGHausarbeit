//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <iostream>
#include "vector.h"
#include "color.h"
#include <string>
#include <map>
#include <vector>
#include "Material.h"

struct Vertex
{
    Vertex();
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT, std::string mtl);
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
    std::string Mtl;
};

struct Texcoord
{
    float s,t;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    Vector Min;
    Vector Max;
};

class Model
{
public:
    Model();
    ~Model();
    const BoundingBox& boundingBox() const;
    bool load( const char* Filename, bool FitSize=true);
    void drawLines() const;
    void drawTriangles() const;
protected:
    void createCube();
    Material* m_pMaterials;
    unsigned int m_MaterialCount;
    Vertex* m_pVertices;
    unsigned int m_VertexCount;
    BoundingBox m_Box;
    std::map<std::string, Material> mtlMap;
};

#endif /* defined(__RealtimeRending__Model__) */
