//
//  Scene.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#include "Scene.h"
#include <stdio>

Scene::Scene(){
}

Scene::~Scene(){
}

bool Scene::parseFile(const char *Filename){
    FILE * object = fopen(Filename, "r");
    if(object == NULL){
        cout << "Konnte Datei nicht oeffnen!" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }
    else{
        while (1) {
            char lineHeader[256];
            result = fscanf(object, "%s", lineHeader);
            if (result == EOF) {
                break;
            }else{
                if (strcmp(lineHeader, "room") == 0){
                    if(strcmp(lineHeader,"size") == 0){
                        Vector size;
                        fscanf(object, "%f %f %f\n", &size.X,&size.Y,&size.Z);
                        v.push_back(vertex); //anders speichern
                    }
                    else if(strcmp(lineHeader, "wallpaper") == 0){
                        if(strcmp(lineHeader, "texture") == 0){
                            fscanf(lineHeader, "");
                        }
                    }
                }else if (strcmp(lineHeader, "vt") == 0){
                    Texcoord vt;
                    fscanf(object, "%f %f\n", &vt.s,&vt.t);
                    vts.push_back(vt);
                }else if (strcmp(lineHeader, "f") == 0){
                    Face face;
                    int match = fscanf(object, "%d/%d %d/%d %d/%d %d/%d", &pidx[0],&tidx[0],&pidx[1],&tidx[1],&pidx[2],&tidx[2],&pidx[3],&tidx[3]);
                    if(match == 6){
                        face.pidx[0] = pidx[0];
                        face.tidx[0] = tidx[0];
                        face.pidx[1] = pidx[1];
                        face.tidx[1] = tidx[1];
                        face.pidx[2] = pidx[2];
                        face.tidx[2] = tidx[2];
                        faces.push_back(face);
                    }else{
                        Face face2;
                        face.pidx[0] = pidx[0];
                        face.tidx[0] = tidx[0];
                        face.pidx[1] = pidx[1];
                        face.tidx[1] = tidx[1];
                        face.pidx[2] = pidx[2];
                        face.tidx[2] = tidx[2];
                        faces.push_back(face);
                        
                        
                        face2.pidx[0] = pidx[2];
                        face2.tidx[0] = tidx[2];
                        face2.pidx[1] = pidx[3];
                        face2.tidx[1] = tidx[3];
                        face2.pidx[2] = pidx[0];
                        face2.tidx[2] = tidx[0];
                        faces.push_back(face2);
                    }
                }
            }
            
            
            
        }

    return true;
}