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
        vector<char*> objNames;
        vector<char*> parentNames;
        vector<char*> fileNames;
        vector<Vector> translations;
        vector<Vector> scalings;
        vector<Vector> rotationsVertices;
        vector<float> rotationAngles;
        while (1) {
            char lineHeader[256];
            result = fscanf(file, "%s", lineHeader);
            if (result == EOF) {
                break;
            }else{
                if(strcmp(lineHeader,"size") == 0){
                    Vector size;
                    fscanf(file, "%f %f %f\n", &size.X,&size.Y,&size.Z);
                }else if(strcmp(lineHeader, "texture") == 0){
                    char bmpFileName[256];
                    fscanf(file, "%s\n", bmpFileName);
                }else if(strcmp(lineHeader, "tiling") == 0){
                    fscanf(file, "%i %i\n",&this->u,&this->v);
                    //Objekt
                }else if(strcmp(lineHeader, "object") == 0){
                    char *objName = new char[256];
                    fscanf(file, "%s\n", objName);
                    objNames.push_back(objName);
                }else if(strcmp(lineHeader, "translation") == 0){
                    Vector translation;
                    fscanf(file, "%a %a %a\n", &translation.X,&translation.Y,&translation.Z);
                    translations.push_back(translation);
                }else if(strcmp(lineHeader, "rotation") == 0){
                    Vector rotationVector;
                    float rotationAngle;
                    fscanf(file, "%f %f %f %f\n",&rotationVector.X,&rotationVector.Y,&rotationVector.Z,&rotationAngle);
                    rotationsVertices.push_back(rotationVector);
                    rotationAngles.push_back(rotationAngle);
                }else if(strcmp(lineHeader, "scaling") == 0){
                    Vector scaling;
                    fscanf(file, "%f %f %f \n", &scaling.X,&scaling.Y,&scaling.Z);
                    scalings.push_back(scaling);
                }else if(strcmp(lineHeader,"model") == 0){
                    char *modelFile = new char[256];
                    fscanf(file, "%s\n",modelFile);
                    fileNames.push_back(modelFile);
                }else if(strcmp(lineHeader, "parent") == 0){
                    char *parent = new char[256];
                    fscanf(file, "%s\n",parent);
                    parentNames.push_back(parent);
                }
                
            }
        }
        
        for(int i = 0; i < objNames.size(); i++){
            Object object;
            object.setName(objNames[i]);
            object.setFileName(fileNames[i]);
            object.setParentName(parentNames[i]);
            object.setScaling(scalings[i]);
            object.setTranslation(translations[i]);
            object.setRotation(rotationsVertices[i], rotationAngles[i]);
            this->objects.push_back(object);
        }
        return true;
    }
}

void Scene::coutObject(){
    cout << "Objektname: " << this->objekts.getName() << endl;
    cout << "Translastion: "<< endl;
    cout << "X: " << this->objekts.getTranslation().X << endl;
    cout << "Y: " << this->objekts.getTranslation().Y << endl;
    cout << "Z: " << this->objekts.getTranslation().Z << endl;
    cout << "Scaling: " << endl;
    cout << "X: " << this->objekts.getScaling().X << endl;
    cout << "Y: " << this->objekts.getScaling().Y << endl;
    cout << "Z: " << this->objekts.getScaling().Z << endl;
    cout << "Rotation: " << endl;
    cout << "X: " << this->objekts.getRotationVector().X << endl;
    cout << "Y: " << this->objekts.getRotationVector().Y << endl;
    cout << "Z: " << this->objekts.getRotationVector().Z << endl;
    cout << "Angle: " << this->objekts.getRotationAngle() << endl;
    cout << "Parent: " << this->objekts.getParentName() << endl;
    cout << "File: " << this->objekts.getFileName() << endl;
}

bool Scene::saveFile(){
    FILE * file = fopen(this->filename, "w");
    Object tmp;
    char *line;
    if(file == NULL){
        cout << "Konnte Datei nicht oeffnen!" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }else{
        for (int i = 0 ; i < this->objects.size(); i++) {
            tmp = this->objects.at(i);
            //Objektname
            fprintf(file, "object ");
            line = tmp.getName();
            fprintf(file, line);
            fprintf(file, "\n{");
            //Tranlation
        }
        return true;
    }
}