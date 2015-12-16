//
//  Scene.hpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include <stdio.h>
#include "vector.h"
#include "Texture.h"
#include "Object.h"
class Scene{
public:
    Vector size;
    Texture wallpaper;
    int u;
    int v;
    Object objekts;
    
    Scene();
    ~Scene();
    bool parseFile(const char* Filename);
};
#endif /* Scene_hpp */
