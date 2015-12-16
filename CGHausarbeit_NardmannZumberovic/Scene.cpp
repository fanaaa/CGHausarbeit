//
//  Scene.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Collaborator: Philipp Nardmann
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
                if(strcmp(lineHeader,"size") == 0){
                        Vector size;
                        fscanf(file, "%f %f %f\n", &size.X,&size.Y,&size.Z);
                    cout << "Room: \n" << endl;
                    cout << "x:" << size.X << " y:" << size.Y << " z:" << size.Z << "\n" << endl;
                }else if(strcmp(lineHeader, "texture") == 0){
                            char bmpFileName[256];
                            fscanf(file, "%s\n", bmpFileName);
                    cout << "Wallpaper: \n" << endl;
                    cout << "Filename: " << bmpFileName << endl;
                }else if(strcmp(lineHeader, "tiling") == 0){
                            fscanf(file, "%i %i\n",&this->u,&this->v);
                    cout << "u: " << this->u << " v:" << this->v << "\n" << endl;
                }else if(strcmp(lineHeader, "object") == 0){
                    char objName[256];
                    fscanf(file, "%s\n", objName);
                    cout << "Objekt: \n" << objName << endl;
                }else if(strcmp(lineHeader, "translation") == 0){
                        fscanf(file, "%a %a %a\n", &this->objekts.translation.X,&this->objekts.translation.Y,&this->objekts.translation.Z);
                    cout << "Translation: \n" << "x:" << this->objekts.translation.X << " y:" << this->objekts.translation.Y << " z:" << this->objekts.translation.Z << endl;
                }else if(strcmp(lineHeader, "rotation") == 0){
                        fscanf(file, "%a %a %a %a\n",&this->objekts.rotationVector.X,&this->objekts.rotationVector.Y,&this->objekts.rotationVector.Z,&this->objekts.rotationAngle);
                    cout << "Rotation: \n" << "x:" << this->objekts.rotationVector.X << " y:" << this->objekts.rotationVector.Y << " z:" << this->objekts.rotationVector.Z << " Angle: " << this->objekts.rotationAngle << endl;
                }else if(strcmp(lineHeader, "scaling") == 0){
                        fscanf(file, "%a %a %a\n", &this->objekts.scaling.X,&this->objekts.scaling.Y,&this->objekts.scaling.Z);
                    cout << "Scaling: \n" << "x:" << this->objekts.scaling.X << " y:" << this->objekts.scaling.Y << " z:" << this->objekts.scaling.Z << endl;
                }else if(strcmp(lineHeader,"model") == 0){
                        char modelFile[256];
                        fscanf(file, "%s\n",modelFile);
                    cout << "Modelfile: " << modelFile << "\n" << endl;
                        
                }
            }
            
            
            
        }

    return true;
}
}