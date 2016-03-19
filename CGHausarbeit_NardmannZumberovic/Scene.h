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
#include "Texture.h"
#include "Object.h"

extern Vector rayOrigin;
extern Vector worldDirection;

class Scene{
public:
    Vector size;
    Texture wallpaper;
    const char* filename;
    int u;
    int v;
    vector<Object> objects;
    char* activeObject;
    
    //Methoden
    Scene();
    Scene(const char* Filename);
    ~Scene();
    bool parseFile();
    bool saveFile();
    Object& getObjectByName(char* objName);
    void setActiveObject();
    
    //Hilfsmethoden saveFile
    void writeObjectHeader(FILE* file,Object obj);
    void writeObjectTransformations(FILE* file, Object obj);
};
#endif /* Scene_h */
