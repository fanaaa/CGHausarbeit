//
//  Scene.hpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Collaborator: Philipp Nardmann
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

using namespace std;


#include "vector.h"
#include "Room.h"
#include "Object.h"

extern Vector rayOrigin;
extern Vector worldDirection;

class Scene{
public:
    const char* filename;
    vector<Object> objects;
    char* activeObject;
    Room room;
 
    
    //Methoden
    Scene();
    Scene(const char* Filename);
    ~Scene();
    bool parseFile();
    bool saveFile();
    Object& getObjectByName(char* objName);
    void setActiveObject();
    
    //Hilfsmethoden saveFile
    void writeRoom(FILE* file,Room room);
    void writeObjectHeader(FILE* file,Object obj);
    void writeObjectTransformations(FILE* file, Object obj);
};
#endif /* Scene_h */
