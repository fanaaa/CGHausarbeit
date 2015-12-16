//
//  Scene.hpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

using namespace std;


#include "vector.h"
#include "Texture.h"
#include "Object.h"

class Scene{
public:
    Vector size;
    Texture wallpaper;
    const char* filename;
    int u;
    int v;
    Object objekts;
    
    Scene(const char* Filename);
    ~Scene();
    bool parseFile();
    bool saveFile();
};
#endif /* Scene_h */
