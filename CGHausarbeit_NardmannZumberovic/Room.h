//
//  Room.hpp
//  RoomTest
//
//  Created by Fana Zumberovic on 19.01.16.
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

#ifndef Room_h
#define Room_h

#include <stdio.h>
#include "vector.h"
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include "ShaderProgram.h"
struct RVertex{
    Vector pos;
    Vector normal;
    float u,v;
};

class Room{
public:
    Room();
    Room(float x, float y, float z);
    ~Room();
    bool loadWall(ShaderProgram shader,float u, float v);
    bool loadFloor(ShaderProgram shader,float u, float v);
    void drawRoom();
private:
    float width;
    float height;
    float length;
    Vector normal;
    GLuint wallVertexBuffer;
    GLuint wallIndexBuffer;
    GLuint floorVertexBuffer;
    GLuint floorIndexBuffer;
};

#endif /* Room_hpp */