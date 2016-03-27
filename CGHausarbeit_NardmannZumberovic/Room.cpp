//
//  Room.cpp
//  
//
//  Created by Fana Zumberovic on 19.01.16.
//  Collaborator: Philipp Nardmann
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

/*
 Klasse zum Abspeichern der Raumwerte.
 Der Raum wird, sofern aus der Datei ausgelesen, abgespeichert.
 Dabei wird die Textur und das entsprechende Tiling verwendet, um Tapete und Raum darzustellen.
 Die Raumbegrenzung werden als Vertices im VBO dargestellt.
 */

#include "Room.h"
#include <string>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/*
 Defaultkonstruktor
 */
Room::Room() : wallTex(new char[512]), floorTex(new char[512]), texturePath(new char[512]), floorName(new char[512]), wallName(new char[512]){
}

/*
 Überladener Konstruktor.
 Angabe des Pfadnamens der Textur-Dateien.
 */
Room::Room(float x, float y, float z) : wallTex(new char[512]), floorTex(new char[512]),floorName(new char[512]), wallName(new char[512]){
    this->width = x;
    this->height = y;
    this->length = z;
    this->texturePath = "./Texturen/";
}

/*
 Destruktor
 */
Room::~Room(){}


/*
 Methode zum Erstellen des VBOs der Wände des Raumes.
 */
bool Room::loadWall(){
    
    Texture wallpaper;

    RVertex wallVertices[12]{
        //first wall
        {Vector(-width,0,-length), Vector(0,0,0), 0, 0},
        {Vector(-width,height,-length), Vector(0,0,0), 0, this->wallTiling.v},
        {Vector(-width,0,length), Vector(0,0,0), this->wallTiling.u, 0},
        {Vector(-width,height,length), Vector(0,0,0), this->wallTiling.u, this->wallTiling.v},
        //second wall
        {Vector(-width,0,length), Vector(0,0,0), 0, 0},
        {Vector(-width,height,length), Vector(0,0,0), 0, this->wallTiling.v},
        {Vector(width,0,length), Vector(0,0,0), this->wallTiling.u, 0},
        {Vector(width,height,length), Vector(0,0,0), this->wallTiling.u, this->wallTiling.v},
        //third wall
        {Vector(width,0,length), Vector(0,0,0), this->wallTiling.u, 0},
        {Vector(width,height,length), Vector(0,0,0), this->wallTiling.u, this->wallTiling.v},
        {Vector(width,0,-length), Vector(0,0,0), 0, 0},
        {Vector(width,height,-length), Vector(0,0,0), 0, this->wallTiling.v},
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
    
    
    if(!wallpaper.LoadFromBMP(this->wallTex)) {
        return false;
    }
    
    wallTexture = wallpaper;

    return true;
}

/*
 Methode zum Erstellen des VBOs des Boden des Raumes.
 */
bool Room::loadFloor(){
    
    Texture wallpaper;
    RVertex floorVertices[4] =
    {
        {Vector(-width,0,-length), Vector(0,0,0), 0, 0},
        {Vector(-width,0,length), Vector(0,0,0), 0, this->floorTiling.v},
        {Vector(width,0,-length), Vector(0,0,0), this->floorTiling.u, 0},
        {Vector(width,0,length), Vector(0,0,0), this->floorTiling.u, this->floorTiling.v}
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
    
    if(!wallpaper.LoadFromBMP(this->floorTex)) {
        return false;
    }
    
    floorTexture = wallpaper;

    return true;
}


/*
 Methode zum Zeichnen des Raumes mit den erstellten VBOs.
 */
void Room::drawRoom(){
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    // setup position & normal pointers
    glVertexPointer(3, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(12));
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(24));
    floorTexture.apply();
    
    
    // we draw our plane
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    // disable states in reverse order
    glDisable(GL_TEXTURE_2D);
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wallVertexBuffer);
    
    // setup position & normal pointers
    glVertexPointer(3, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(12));
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(RVertex), BUFFER_OFFSET(24));
    wallTexture.apply();
    
    // we draw our plane
    glDrawElements( GL_TRIANGLES, 24, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    
    // disable states in reverse order
    glDisable(GL_TEXTURE_2D);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }


/*
 Set-Methoden
 */
void Room::setFloorTex(char *floor){
    this->floorName = floor;
    strcat(this->floorTex, this->texturePath);
    strcat(this->floorTex, floor);
    
}

void Room::setWallTex(char *wall){
    this->wallName = wall;
    strcat(this->wallTex, this->texturePath);
    strcat(this->wallTex, wall);
}

void Room::setFloorTiling(float u, float v){
    this->floorTiling.u = u;
    this->floorTiling.v = v;
}

void Room::setWallTiling(float u, float v){
    this->wallTiling.u = u;
    this->wallTiling.v = v;
}

/*
 Get-Methoden
 */
char* Room::getFloorTex(){
    return this->floorTex;
}

char* Room::getWallTex(){
    return this->wallTex;
}

char* Room::getFloorName(){
    return this->floorName;
}

char* Room::getWallName(){
    return this->wallName;
}

float Room::getWidth(){
    return this->width;
}
float Room::getHeight(){
    return this->height;
}
float Room::getLength(){
    return this->length;
}

tiling Room::getWallTiling(){
    return this->wallTiling;
}

tiling Room::getFloorTiling(){
    return this->floorTiling;
}