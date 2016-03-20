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

Scene::Scene(){

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
                    this->room = Room(size.X, size.Y, size.Z);
                }else if(strcmp(lineHeader, "walltex") == 0){
                    char* wallTex = new char[256];
                    fscanf(file, "%s\n", wallTex);
                    room.setWallTex(wallTex);
                }else if(strcmp(lineHeader, "walltiling") == 0){
                    tiling tmp;
                    fscanf(file, "%f %f\n",&tmp.u,&tmp.v);
                    room.setWallTiling(tmp.u, tmp.v);
                }else if(strcmp(lineHeader, "floortex") == 0){
                    char* floorTex = new char[256];
                    fscanf(file, "%s\n", floorTex);
                    room.setFloorTex(floorTex);
                }else if(strcmp(lineHeader, "floortiling") == 0){
                    tiling tmp2;
                    fscanf(file, "%f %f\n",&tmp2.u,&tmp2.v);
                    room.setFloorTiling(tmp2.u, tmp2.v);
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

//Aktuelle Positionen in Datei speichern
bool Scene::saveFile(){
    FILE * file = fopen(this->filename, "w");
    if(file == NULL){
        cout << "Konnte Datei nicht oeffnen!" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }else{
        //Room
        this->writeRoom(file, this->room);
        //Object
        for(Object tmp : this->objects){
            this->writeObjectHeader(file,tmp);
            this->writeObjectTransformations(file, tmp);
        }
        fclose(file);
        return true;
    }
}



Object& Scene::getObjectByName(char* objName){
    Object& tmp = this->objects.at(0);
    for (Object& tmp : this->objects) {
        if (!std::strcmp(tmp.getName(), objName)) {
            return tmp;
        }
    }
    return tmp;
}

void Scene::setActiveObject(){
    for(Object& tmp : this->objects){
        if(tmp.objectIsClicked()){
            this->activeObject = tmp.getName();
            break;
        }
    }
}

void Scene::writeObjectHeader(FILE *file, Object obj){
    char objectHeader[124] = "";
    strcat(objectHeader, "object ");
    strcat(objectHeader, obj.getName());
    strcat(objectHeader, "\n");
    fprintf(file, objectHeader);
}

void Scene::writeRoom(FILE *file,Room room){
    char roomWrite[1024] = "";
    char size[128] = "";
    char walltex[128] = "";
    char walltiling[128] = "";
    char floortex[128] = "";
    char floortiling[128] = "";
    sprintf(size, "%s %f %f %f","size",room.getWidth(),room.getHeight(),room.getLength());
    strcat(size, "\n");
    sprintf(walltex, "%s %s","walltex",room.getWallName());
    strcat(walltex, "\n");
    sprintf(walltiling, "%s %f %f","walltiling",room.getWallTiling().u,room.getWallTiling().v);
    strcat(walltiling, "\n}\n");
    sprintf(floortex, "%s %s","floortex",room.getFloorName());
    sprintf(floortiling, "%s %f %f","floortiling",room.getWallTiling().u,room.getWallTiling().v);
    
    strcat(roomWrite, "room\n{\n");
    strcat(roomWrite, size);
    strcat(roomWrite, "wallpaper\n{\nwall\n{\n");
    strcat(roomWrite, walltex);
    strcat(roomWrite, walltiling);
    strcat(roomWrite, "floor\n{\n");
    strcat(roomWrite, floortex);
    strcat(roomWrite, "\n");
    strcat(roomWrite, floortiling);
    strcat(roomWrite, "\n}\n}\n}\n");
    fprintf(file, roomWrite);
}


void Scene::writeObjectTransformations(FILE *file, Object obj){
    char objectBody[1024] = "";
    char translation [128] = "";
    char rotation [128] = "";
    char scaling [128]= "";
    char fileName[128] = "";
    char parent[128] = "";
    //Translation
    sprintf(translation, "%s %f %f %f", "translation",obj.getTranslation().X, obj.getTranslation().Y,obj.getTranslation().Z);
    strcat(translation, "\n");
    //Rotation
    sprintf(rotation, "%s %f %f %f %f","rotation ", obj.getRotationVector().X,obj.getRotationVector().Y,obj.getRotationVector().Z,obj.getRotationAngle());
    strcat(rotation, "\n");
    //Scaling
    sprintf(scaling, "%s %f %f %f","scaling ", obj.getScaling().X, obj.getScaling().Y,obj.getScaling().Z);
    strcat(scaling, "\n");
    //Dateiname
    strcat(fileName, "model ");
    strcat(fileName, obj.getName());
    strcat(fileName, ".obj");
    strcat(fileName, "\n");
    //Parent
    strcat(parent, "parent ");
    strcat(parent, obj.getParentName());
    strcat(parent, "\n");
    //Objekt zusammensetzen
    strcat(objectBody, "{\n");
    strcat(objectBody, translation);
    strcat(objectBody, rotation);
    strcat(objectBody, scaling);
    strcat(objectBody, fileName);
    strcat(objectBody, parent);
    strcat(objectBody, "}\n");
    fprintf(file, objectBody);
}

