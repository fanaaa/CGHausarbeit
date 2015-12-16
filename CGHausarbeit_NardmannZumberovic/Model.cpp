//
//  Model.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#include "Model.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <algorithm>
#include <float.h>
#include "ObjParser.h"

//#define WITH_CUBE

Vertex::Vertex()
{
    
}

Vertex::Vertex( const Vector& p, const Vector& n, float TexS, float TexT, std::string mtl)
{
    Position = p;
    Normal = n;
    TexcoordS = TexS;
    TexcoordT = TexT;
    Mtl = mtl;
}

BoundingBox::BoundingBox()
{
}
BoundingBox::BoundingBox( const Vector& min, const Vector& max) : Min(min), Max(max)
{
}

Model::Model() : m_pVertices(NULL), m_pMaterials(NULL), m_MaterialCount(0), m_VertexCount(0)
{

}

Model::~Model()
{
    if( m_pVertices)
        delete [] m_pVertices;
    if(m_pMaterials)
        delete [] m_pMaterials;
}

bool Model::load( const char* Filename, bool FitSize)
{
#ifdef WITH_CUBE
    createCube();
    
    ObjParser parser;
    vector<Vertex> vertices = parser.parseObjFile(Filename, true);
    
    m_Box = parser.getBoundingBox();
    
    long oldSize = m_VertexCount;
    long newSize = oldSize + vertices.size();
    //long newSize = vertices.size();
    Vertex* newArr = new Vertex[newSize];
    
    m_VertexCount = newSize;
    
    memcpy( newArr, m_pVertices, oldSize * sizeof(Vertex) );
    
    delete[] m_pVertices;
    m_pVertices = newArr;
    
    for(int i = 0; i < vertices.size(); i++) {
        m_pVertices[oldSize + i] = vertices[i];
       // m_pVertices[i] = vertices[i];
    }
#else
    ObjParser parser;
    vector<Vertex> vertices = parser.parseObjFile(Filename, false);
    
    m_Box = parser.getBoundingBox();
    m_pVertices = new Vertex[vertices.size()];
    m_VertexCount = vertices.size();
    
    for(int i = 0; i < vertices.size(); i++) {
        m_pVertices[i] = vertices[i];
    }
#endif
    
    mtlMap = parser.getMtlMap();
    
    return true;
}

void Model::createCube()
{
    Vector PositionsFromFile[8] =
    {
        Vector(-1, -1, 1),
        Vector(-1,  1, 1),
        Vector( 1, -1, 1),
        Vector( 1,  1, 1),
        Vector( 1, -1, -1),
        Vector( 1,  1, -1),
        Vector(-1, -1, -1),
        Vector(-1, 1, -1)
    };

    //Moved Texcoord to header

    Texcoord TexcoordFromFile[20] =
    {
        { 0, 1 },
        { 1, 0 },
        { 0, 0 },
        { 0, 0 },
        { 1, 1 },
        { 0, 1 },
        { 1, 1 },
        { 0, 0 },
        { 1, 0 },
        { 1, 0 },
        { 0, 1 },
        { 1, 1 },
        { 1, 0 },
        { 0, 0 },
        { 1, 1 },
        { 0, 1 },
        { 0, 0 },
        { 1, 0 },
        { 0, 1 },
        { 1, 1 }
    };
    struct Face
    {
        unsigned int pidx[3], tidx[3];
    };
    Face FacesFromFile[12]
    {
        //face 1
        {3,4,2,   9, 12, 6},
        {3,2,1,   9, 6, 3},
        //face 2
        {5,6,4,   13,15,11},
        {5,4,3,   13,11,8},
        //face 3
        {7,8,6,   18,20,16},
        {7,6,5,   18,16,14},
        //face 4
        {1,2,8,   2,5,19},
        {1,8,7,   2,19,17},
        //face 5
        {4,6,8,   10,15,19},
        {4,8,2,   10,19,4},
        //face 6
        {5,3,1,   13,7,1},
        {5,1,7,   13,1,17}
        
    };
    
    
    m_pVertices = new Vertex[12*3];
    m_VertexCount = 12*3;
    for(int i=0; i<12; i++ )
    {
        unsigned int PosIdx0 = FacesFromFile[i].pidx[0]-1;
        unsigned int PosIdx1 = FacesFromFile[i].pidx[1]-1;
        unsigned int PosIdx2 = FacesFromFile[i].pidx[2]-1;
        
        unsigned int TexIdx0 = FacesFromFile[i].tidx[0]-1;
        unsigned int TexIdx1 = FacesFromFile[i].tidx[1]-1;
        unsigned int TexIdx2 = FacesFromFile[i].tidx[2]-1;
        
        Vector a = m_pVertices[i*3].Position = PositionsFromFile[ PosIdx0];
        Vector b = m_pVertices[i*3+1].Position = PositionsFromFile[ PosIdx1];
        Vector c = m_pVertices[i*3+2].Position = PositionsFromFile[ PosIdx2];
        
        m_pVertices[i*3].TexcoordS = TexcoordFromFile[TexIdx0].s;
        m_pVertices[i*3+1].TexcoordS = TexcoordFromFile[TexIdx1].s;
        m_pVertices[i*3+2].TexcoordS = TexcoordFromFile[TexIdx2].s;
        
        m_pVertices[i*3].TexcoordT = TexcoordFromFile[TexIdx0].t;
        m_pVertices[i*3+1].TexcoordT = TexcoordFromFile[TexIdx1].t;
        m_pVertices[i*3+2].TexcoordT = TexcoordFromFile[TexIdx2].t;
        
        Vector normal = (b-a).cross(c-a);
        normal.normalize();
        
        m_pVertices[i*3].Normal =
        m_pVertices[i*3+1].Normal =
        m_pVertices[i*3+2].Normal = normal;
    }
    
    printf( "Vertices:\n");
    for( unsigned int i=0; i<m_VertexCount; i++)
    {
        printf( "%2i: ", i);
        printf( "p(%2.1f, %2.1f, %2.1f) ", m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z );
        printf( "n(%2.1f, %2.1f, %2.1f) ", m_pVertices[i].Normal.X, m_pVertices[i].Normal.Y, m_pVertices[i].Normal.Z );
        printf( "t(%2.1f, %2.1f)\n", m_pVertices[i].TexcoordS, m_pVertices[i].TexcoordT );
        
    }
}


const BoundingBox& Model::boundingBox() const
{
    return m_Box;
}

void Model::drawLines() const
{
    glBegin(GL_LINES);
        for(unsigned int i = 0; i < m_VertexCount; i=i+3) {
            glColor3f(   1.0f,  0.0f,  1.0f);  // Violet
            
            //1 to
            glVertex3f(m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z);
            //2
            glVertex3f(m_pVertices[i+1].Position.X, m_pVertices[i+1].Position.Y, m_pVertices[i+1].Position.Z);
            
            //2 to
            glVertex3f(m_pVertices[i+1].Position.X, m_pVertices[i+1].Position.Y, m_pVertices[i+1].Position.Z);
            //3
            glVertex3f(m_pVertices[i+2].Position.X, m_pVertices[i+2].Position.Y, m_pVertices[i+2].Position.Z);
            
            //3 to
            glVertex3f(m_pVertices[i+2].Position.X, m_pVertices[i+2].Position.Y, m_pVertices[i+2].Position.Z);
            //1
            glVertex3f(m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z);
        }
    glEnd();
}

void Model::drawTriangles() const
{
    
    for(unsigned int i = 0; i < m_VertexCount; i=i+3) {
        string curMtl = m_pVertices[i].Mtl;
        
        if(curMtl.compare("") != 0) { //A material was set
            if(mtlMap.find(curMtl) != mtlMap.end()) { //material was already created
                Material mtl = mtlMap.at(curMtl);
                
                mtl.getTexture().apply();
                
                float diff[4] = {mtl.getDiffuseColor().R, mtl.getDiffuseColor().G, mtl.getDiffuseColor().B, 1};
                float amb[4] = {mtl.getAmbientColor().R, mtl.getAmbientColor().G, mtl.getAmbientColor().B, 1};
                float spec[4] = {mtl.getSpecularColor().R, mtl.getSpecularColor().G, mtl.getSpecularColor().B, 1};
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
                glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
                glMateriali(GL_FRONT, GL_SHININESS, mtl.getSpecularExponent());
                glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            }
        }
        
        glBegin(GL_TRIANGLES);
        
        glNormal3f(m_pVertices[i].Normal.X, m_pVertices[i].Normal.Y, m_pVertices[i].Normal.Z);
        
        //1 to
        glTexCoord2f(m_pVertices[i].TexcoordS, m_pVertices[i].TexcoordT);
        glVertex3f(m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z);
        //2 to
        glTexCoord2f(m_pVertices[i+1].TexcoordS, m_pVertices[i+1].TexcoordT);
        glVertex3f(m_pVertices[i+1].Position.X, m_pVertices[i+1].Position.Y, m_pVertices[i+1].Position.Z);
        //3
        glTexCoord2f(m_pVertices[i+2].TexcoordS, m_pVertices[i+2].TexcoordT);
        glVertex3f(m_pVertices[i+2].Position.X, m_pVertices[i+2].Position.Y, m_pVertices[i+2].Position.Z);
        
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
    }
    
}



