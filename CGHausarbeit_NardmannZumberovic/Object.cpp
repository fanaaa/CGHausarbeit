//
//  Object.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Collaborator: Philipp Nardmann
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//
#define BUFFER_OFFSET(i) ((char *)NULL + (i)) // Kein Off-Set; alle Vektoren werden gezeichnet

using namespace std;
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

#include "Object.h"
#include <float.h>
#include <assert.h>
#include <math.h>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>

Vertex::Vertex(){}

Vertex::Vertex( const Vector& p, const Vector& n, float u, float v){
    Position = p;
    Normal = n;
    u = u;
    v = v;
}

Object::Object() : name(new char[256]), parentName(new char[256]), fileName(new char[256]), rotationAngle(0), rotationVector(), scaling(), translation(), indexSize(0), vertexSize(0){

}

Object::~Object(){
}

bool Object::load(const char *Filename, Vector translation, Vector scaling, Vector rotationVector, float rotationAngle){
    
    this->translation = translation;
    this->scaling = scaling;
    this->rotationVector = rotationVector;
    this->rotationAngle = rotationAngle;
    
    struct Texcoord
    {
        float s,t;
    };
    
    struct Face
    {
        unsigned int pidx[3], tidx[3];
    };
    
    
    int result;
    Vector v;
    Vertex vertex;
    vector<Vertex> vertexBuffer;
    vector<unsigned int> indexBuffer;
    vector<Texcoord> vts;
    unsigned int pidx[4];
    unsigned int tidx[4];
    
    FILE * object = fopen(Filename, "r");
    if(object == NULL){
        cout << " Konnte Datei nicht oeffnen!" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }else{
        while (1) {
            char lineHeader[256];
            result = fscanf(object, "%s", lineHeader);
            if(result == EOF){
                break;
            }else{
                if (strcmp(lineHeader, "v") == 0){
                    fscanf(object, "%a %a %a\n", &vertex.Position.X,&vertex.Position.Y,&vertex.Position.Z);
                    m_Vertices.push_back(vertex);
                }else if (strcmp(lineHeader, "vt") == 0){
                    Texcoord vt;
                    fscanf(object, "%a %a\n", &vt.s,&vt.t);
                    vts.push_back(vt);
                }else if (strcmp(lineHeader, "f") == 0){
                    Vertex vertex1;
                    Vertex vertex2;
                    Vertex vertex3;
                    Vertex vertex4;
                    Vector tmp1;
                    Vector tmp2;
                    Vector tmp3;
                    Vector tmp4;
                    int match = fscanf(object, "%d/%d %d/%d %d/%d %d/%d", &pidx[0],&tidx[0],&pidx[1],&tidx[1],&pidx[2],&tidx[2],&pidx[3],&tidx[3]);
                    
                    //Texturkoordinaten speichern
                    if(match == 6){
                        indexBuffer.push_back(pidx[0]-1);
                        indexBuffer.push_back(pidx[1]-1);
                        indexBuffer.push_back(pidx[2]-1);
                    }
                    if(match == 8){
                        indexBuffer.push_back(pidx[0]-1);
                        indexBuffer.push_back(pidx[1]-1);
                        indexBuffer.push_back(pidx[2]-1);
                        
                        indexBuffer.push_back(pidx[2]-1);
                        indexBuffer.push_back(pidx[3]-1);
                        indexBuffer.push_back(pidx[0]-1);
                        
                    }
                }
                
                
            }
        }
        
        //Normale berechnen
        Vector normal;
        for (int i = 0; i < indexBuffer.size() / 3; i++) {
            normal = (m_Vertices.at(indexBuffer[3*i+1]).Position - m_Vertices.at(indexBuffer[3*i]).Position).cross(m_Vertices.at(indexBuffer[3*i+2]).Position - m_Vertices.at(indexBuffer[3*i]).Position);
            
            normal.normalize();
            m_Vertices[indexBuffer[3*i]].Normal = normal;
            m_Vertices[indexBuffer[3*i+1]].Normal = normal;
            m_Vertices[indexBuffer[3*i+2]].Normal = normal;
            
        }
        
        
        //Objekte Grundskalieren
        this->createBoundingBox(m_Vertices);
        float scale = 5 / ((m_Box.Min - m_Box.Max).length());
        for(int i = 0; i < m_Vertices.size(); i++){
            m_Vertices[i].Position = m_Vertices[i].Position * scale;
        }
        
        this->indexSize = indexBuffer.size();
        
        glGenBuffers( 1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW );
        
        glGenBuffers( 1, &m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexBuffer.size(), &indexBuffer[0], GL_STATIC_DRAW);
        fclose(object);
        return true;
    }
    
}

void Object::draw(){
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    // setup position & normal pointers
    
    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
    
    glDrawElements( GL_TRIANGLES, this->indexSize , GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::createBoundingBox(std::vector<Vertex> vertices){
    m_Box.Max.X = m_Box.Min.X = vertices[0].Position.X;
    m_Box.Max.Y = m_Box.Min.Y = vertices[0].Position.Y;
    m_Box.Max.Z = m_Box.Min.Z = vertices[0].Position.Z;
    
    for(int i = 0; i < vertices.size(); i++) {
        Vector vertex = vertices[i].Position;
        
        if(m_Box.Min.X < vertex.X) m_Box.Min.X = vertex.X;
        if(m_Box.Min.Y < vertex.Y) m_Box.Min.Y = vertex.Y;
        if(m_Box.Min.Z < vertex.Z) m_Box.Min.Z = vertex.Z;
        
        if(m_Box.Max.X > vertex.X) m_Box.Max.X = vertex.X;
        if(m_Box.Max.Y > vertex.Y) m_Box.Max.Y = vertex.Y;
        if(m_Box.Max.Z > vertex.Z) m_Box.Max.Z = vertex.Z;
    }
}

void Object::update(){
}

//setter
void Object::setRotation(Vector &rotationVector, float rotationAngle){
    this->rotationVector = rotationVector;
    this->rotationAngle = rotationAngle;
}

void Object::setTranslation(Vector &translation){
    this->translation = translation;
}

void Object::setScaling(Vector& scaling){
    this->scaling = scaling;
}

void Object::setName(char *newName){
    this->name = newName;
}

void Object::setParentName(char *parentName){
    this->parentName = parentName;
}

void Object::setFileName(char *file){
    this->fileName = file;
}

//getter
Vector& Object::getTranslation(){
    return this->translation;
}

Vector& Object::getRotationVector(){
    return this->rotationVector;
}

float Object::getRotationAngle(){
    return this->rotationAngle;
}

Vector& Object::getScaling(){
    return this->scaling;
}

char* Object::getName(){
    return this->name;
}

char* Object::getParentName(){
    return this->parentName;
}

char* Object::getFileName(){
    return this->fileName;
}

char* Object::getVectorAsString(Vector& vector){
    char* string;
    
    return string;
}
