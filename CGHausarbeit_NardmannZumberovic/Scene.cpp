//
//  Scene.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#include "Scene.h"
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <float.h>

using namespace std;

Scene::Scene(const char* Filename){
    this->filename = Filename;
}

Scene::~Scene(){
}

bool Scene::parseFile(){
    int result;
    FILE * file = fopen(this->filename, "r");
    if(file == NULL){
        cout << "Konnte Datei nicht oeffnen!" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }
    else{
        while (1) {
            char lineHeader[256];
            result = fscanf(file, "%s", lineHeader);
            if (result == EOF) {
                break;
            }else{
                if (strcmp(lineHeader, "room") == 0){
                    if(strcmp(lineHeader,"size") == 0){
                        Vector size;
                        fscanf(file, "%f %f %f\n", &size.X,&size.Y,&size.Z);
                        cout << "x: %i y: %i z: %i" << size.X,size.Y,size.Z;
                        cout << endl;
                    }
                    else if(strcmp(lineHeader, "wallpaper") == 0){
                        if(strcmp(lineHeader, "texture") == 0){
                            char* bmpFileName;
                            fscanf(file, "%s\n", bmpFileName);
                        }
                        else if(strcmp(lineHeader, "tiling") == 0){
                            fscanf(file, "%i %i\n",&this->u,&this->v);
                        }
                    }
                }else if (strcmp(lineHeader, "object") == 0){
                    if(strcmp(lineHeader, "translation") == 0){
                        fscanf(file, "%a %a %a\n", &this->objekts.translation.X,&this->objekts.translation.Y,&this->objekts.translation.Z);
                    }
                    else if(strcmp(lineHeader, "rotation") == 0){
                        fscanf(file, "%a %a %a %a\n",&this->objekts.rotationVector.X,&this->objekts.rotationVector.Y,&this->objekts.rotationVector.Z,&this->objekts.rotationAngle);
                    }
                    else if(strcmp(lineHeader, "scaling") == 0){
                        fscanf(file, "%a %a %a\n", &this->objekts.scaling.X,&this->objekts.scaling.Y,&this->objekts.scaling.Z);
                    }
                    else if(strcmp(lineHeader,"model") == 0){
                        char* modelFile;
                        fscanf(file, "%s\n",modelFile);
                        
                    }
                    else{
                        char* parent;
                        fscanf(file, "%s\n",parent);
                        if(parent == NULL){
                            //Wurzeloperation
                        }
                    }
                }
            }
            
            
            
        }

    return true;
}
}