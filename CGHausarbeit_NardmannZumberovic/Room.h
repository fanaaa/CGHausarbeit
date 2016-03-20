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
#include "Texture.h"
struct RVertex{
    Vector pos;
    Vector normal;
    float u,v;
};

struct tiling{
    float u,v;
};

class Room{
public:
    Room();
    Room(float x, float y, float z);
    ~Room();
    bool loadWall();
    bool loadFloor();
    void drawRoom();
    
    //setter
    void setFloorTiling(float u,float v);
    void setWallTiling(float u,float v);
    void setTexturePath(const char* texPath);
    void setFloorTex(char* floor);
    void setWallTex(char* wall);
    
    //getter
    char* getFloorTex();
    char* getWallTex();
    float getWidth();
    float getHeight();
    float getLength();
    char* getFloorName();
    char* getWallName();
    tiling getWallTiling();
    tiling getFloorTiling();
private:
    float width;
    float height;
    float length;
    char* texturePath;
    char* floorTex;
    char* wallTex;
    char* floorName;
    char* wallName;
    tiling wallTiling;
    tiling floorTiling;
    Vector normal;
    GLuint wallVertexBuffer;
    GLuint wallIndexBuffer;
    GLuint floorVertexBuffer;
    GLuint floorIndexBuffer;
    Texture wallTexture;
    Texture floorTexture;
};

#endif /* Room_hpp */
