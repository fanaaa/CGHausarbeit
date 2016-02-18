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
#include <vector>
#include "vector.h"
#include "BoundingBox.h"

struct Vertex
{
public:
    Vertex();
    Vertex( const Vector& p, const Vector& n, float u, float v);
    Vector Position;
    Vector Normal;
    float  u;
    float  v;
};

class Object{
public:
    Object();
    ~Object();
    bool load( const char* Filename,Vector translation,Vector scaling,Vector rotationVector,float rotationAngle);
    const BoundingBox& getBoundingBox() const;
    void createBoundingBox(std::vector<Vertex> vertices);
    void draw();
    void update();
    
    //get-Methoden
    Vector& getTranslation();
    Vector& getRotationVector();
    float getRotationAngle();
    Vector& getScaling();
    char* getName();
    char* getParentName();
    char* getFileName();
    char* getVectorAsString(Vector& vector);
    
    //set-Methoden
    void setTranslation(Vector& translation);
    void setRotation(Vector& rotationVector,float rotationAngle);
    void setScaling(Vector& scaling);
    void setName(char* name);
    void setParentName(char* parentName);
    void setFileName(char* file);
    
protected:
    long indexSize;
    long vertexSize;
    BoundingBox m_Box;
    Vector translation;
    Vector scaling;
    Vector rotationVector;
    vector<Vertex> m_Vertices;
    float rotationAngle;
    char* name;
    char* parentName;
    char* fileName;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;

};
#endif /* Object_h */
