//
//  Room.cpp
//  RoomTest
//
//  Created by Fana Zumberovic on 19.01.16.
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

#include "Room.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Room::Room(){}

Room::Room(float x, float y, float z){
    this->width = x;
    this->height = y;
    this->length = z;
}

Room::~Room(){}

bool Room::loadWall(ShaderProgram shader,float u, float v){
    
    shader.activate();
    GLint loc = shader.getParameterID("LightPos");
    glUniform3f(loc, 1.0f, 4.0f, 1.0f);
    loc = shader.getParameterID("LightColor");
    glUniform3f(loc, 0.5f, 0.5f, 0.5f);
    loc = shader.getParameterID("DiffColor");
    glUniform3f(loc, 1.0f, 1.0f, 1.0f);
    loc = shader.getParameterID("SpecColor");
    glUniform3f(loc, 0.1f, 0.1f, 0.1f);
    loc = shader.getParameterID("AmbientColor");
    glUniform3f(loc, 0.2f, 0.2f, 0.2f);
    loc = shader.getParameterID("SpecExp");
    glUniform1f(loc, 1);
    shader.deactivate();

    RVertex wallVertices[12]{
        //first wall
        {Vector(-width,0,-length), Vector(0,0,0), 0, 0},
        {Vector(-width,height,-length), Vector(0,0,0), 0, v},
        {Vector(-width,0,length), Vector(0,0,0), u, 0},
        {Vector(-width,height,length), Vector(0,0,0), u, v},
        //second wall
        {Vector(-width,0,length), Vector(0,0,0), 0, 0},
        {Vector(-width,height,length), Vector(0,0,0), 0, v},
        {Vector(width,0,length), Vector(0,0,0), u, 0},
        {Vector(width,height,length), Vector(0,0,0), u, v},
        //third wall
        {Vector(width,0,length), Vector(0,0,0), u, 0},
        {Vector(width,height,length), Vector(0,0,0), u, v},
        {Vector(width,0,-length), Vector(0,0,0), 0, 0},
        {Vector(width,height,-length), Vector(0,0,0), 0, v},
    };
    
    unsigned int wallIndices[18] ={0,1,3,0,3,2,6,4,5,6,5,7,8,9,11,8,11,10,};
    
    this->normal = Vector(0,0,0);
    for(int i = 0; i < 18/3; i++) {
        normal = (wallVertices[wallIndices[3*i+1]].pos - wallVertices[wallIndices[3*i]].pos).cross(wallVertices[wallIndices[3*i+2]].pos - wallVertices[wallIndices[3*i]].pos);
        normal.normalize();
        wallVertices[wallIndices[3*i]].normal += normal;
        wallVertices[wallIndices[3*i+1]].normal += normal;
        wallVertices[wallIndices[3*i+2]].normal += normal;
    }
    
    for(int i = 0; i < sizeof(wallVertices)/sizeof(RVertex); i++) {
        wallVertices[i].normal.normalize();
    }
    // gpu vertexBuffer
    glGenBuffers( 1, &wallVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wallVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RVertex)*16, wallVertices, GL_STATIC_DRAW );
    // gpu indexBuffer
    glGenBuffers( 1, &wallIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*24, wallIndices, GL_STATIC_DRAW);
    

    return true;
}

bool Room::loadFloor(ShaderProgram shader,float u, float v){
    
    shader.activate();
    GLint loc = shader.getParameterID("LightPos");
    glUniform3f(loc, 1.0f, 4.0f, 1.0f);
    loc = shader.getParameterID("LightColor");
    glUniform3f(loc, 0.5f, 0.5f, 0.5f);
    loc = shader.getParameterID("DiffColor");
    glUniform3f(loc, 1.0f, 1.0f, 1.0f);
    loc = shader.getParameterID("SpecColor");
    glUniform3f(loc, 0.1f, 0.1f, 0.1f);
    loc = shader.getParameterID("AmbientColor");
    glUniform3f(loc, 0.2f, 0.2f, 0.2f);
    loc = shader.getParameterID("SpecExp");
    glUniform1f(loc, 1);
    shader.deactivate();

    RVertex floorVertices[4] =
    {
        {Vector(-width,0,-length), Vector(0,0,0), 0, 0},
        {Vector(-width,0,length), Vector(0,0,0), 0, v},
        {Vector(width,0,-length), Vector(0,0,0), u, 0},
        {Vector(width,0,length), Vector(0,0,0), u, v}
    };
    
    unsigned int floorIndices[6] = {0,1,3,0,3,2};
    
    for(int i = 0; i < 6/3; i++) {
        normal = (floorVertices[floorIndices[3*i+1]].pos - floorVertices[floorIndices[3*i]].pos).cross(floorVertices[floorIndices[3*i+2]].pos - floorVertices[floorIndices[3*i]].pos);
        normal.normalize();
        floorVertices[floorIndices[3*i]].normal += normal;
        floorVertices[floorIndices[3*i+1]].normal += normal;
        floorVertices[floorIndices[3*i+2]].normal += normal;
    }
    
    for(int i = 0; i < sizeof(floorVertices)/sizeof(RVertex); i++) {
        floorVertices[i].normal.normalize();
    }
    
    // gpu vertexBuffer
    glGenBuffers( 1, &floorVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RVertex)*4, floorVertices, GL_STATIC_DRAW );
    // gpu indexBuffer
    glGenBuffers( 1, &floorIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6, floorIndices, GL_STATIC_DRAW);
    

    return true;
}

void Room::drawRoom(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // array buffers for walls
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wallVertexBuffer);
    
    
    // position & normal
    glVertexPointer(3, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(12));
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(24));
    
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    // draw walls
    glDrawElements( GL_TRIANGLES, 18, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    
   
    //array buffers for floor
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    
    //position & normal
    glVertexPointer(3, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(12));
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(24));

    
    //draw floor
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

    // disable states in reverse order
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // set modulate as default behaviour for unit 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}