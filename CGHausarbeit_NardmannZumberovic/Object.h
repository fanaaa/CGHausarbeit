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
#include "Matrix.h"
#include "color.h"

extern Vector rayOrigin;
extern Vector worldDirection;

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
    void vDraw();

    void update();
    bool objectIsClicked();
    
    //get-Methoden
    Vector& getTranslation();
    Vector& getRotationVector();
    float getRotationAngle();
    Vector& getScaling();
    char* getName();
    char* getParentName();
    char* getFileName();
    char* getVectorAsString(Vector& vector);
    Matrix getMatrix();
    bool getActive();
    
    //set-Methoden
    void setTranslation(Vector& translation);
    void setRotation(Vector& rotationVector,float rotationAngle);
    void setScaling(Vector& scaling);
    void setName(char* name);
    void setParentName(char* parentName);
    void setFileName(char* file);
    void setMatrix(Matrix newMatrix);
    void setActive(bool boolean);
    
public:
    Vector hp;
    long indexSize;
    long vertexSize;
    bool isActiveObject;
    BoundingBox m_Box;
    Vector translation;
    Vector scaling;
    Vector rotationVector;
    vector<Vertex> m_Vertices;
    vector<unsigned int>m_Indices;
    vector<Color>m_color;
    float rotationAngle;
    char* name;
    char* parentName;
    char* fileName;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_colorBuffer;
    Matrix m_transMatrix;

};
#endif /* Object_h */
