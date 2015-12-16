//
//  ObjParser.cpp
//  Praktikum3
//
//  Created by Boss, Mark on 15.04.15.
//  Copyright (c) 2015 Boss, Mark. All rights reserved.
//

#include "ObjParser.h"
#include <fstream>
#include <limits>
#include <iostream>
#include "Material.h"

using namespace std;

#define SCALE_FACTOR 3.f;

ObjParser::ObjParser() {
    
}

vector<Vertex> ObjParser::parseObjFile(const char* Filename, bool fit) {
    ifstream file(Filename);
    string line;
    
    string curGroup = "";
    string mtlLib = "";
    string curUsedMtl = "";
    
    while (getline(file, line)) {
        trim(line);
        vector<string> data = split(line, ' ');
        
        if(data[0].compare("g") == 0) { // group
            curGroup = data[1]; //TODO use group?
            curUsedMtl = "";
        } else if (data[0].compare("v") == 0) { //Vertex
            float x = stof(data[1]);
            float y = stof(data[2]);
            float z = stof(data[3]);
            
            tempPositionVec.push_back(Vector(x, y, z));
        } else if (data[0].compare("vt") == 0) { //Texcoord
            tempTexCoordVec.push_back(Texcoord());
            tempTexCoordVec.back().s = stof(data[1]);
            tempTexCoordVec.back().t = stof(data[2]);
        } else if (data[0].compare("f") == 0) { //all vertices and tex coords of this group are already parsed in
            vector<Vector> positions;
            vector<Texcoord> texCoords;
            
            vector<int> vertexIndices;
            vector<int> texIndices;
            
            for(int i = 1; i < data.size(); i++) {
                vector<string> splittedField = split(data[i], '/');
                if(splittedField.size() > 2 || splittedField.size() == 0) {
                    throw "Unsupported area format";
                }
                
                if(data.size() - 1 == 3) { //Triangle
                    //Setting the positions and texcoords is possible without a conversion
                    int vertexIndex = stoi(splittedField[0]) - 1; //offset by 1
                    positions.push_back(tempPositionVec[vertexIndex]);
                    
                    int texIndex = stoi(splittedField[1]) - 1; //offset by 1
                    texCoords.push_back(tempTexCoordVec[texIndex]);
                } else if (data.size() -1 == 4) { //Quad
                    //save the indices of the vertices and texcoords so we can split it later
                    vertexIndices.push_back(stoi(splittedField[0]) - 1); //offset by 1
                    texIndices.push_back(stoi(splittedField[1]) - 1); //offset by 1
                } //Polygons aren't supported
                
            }
            
            if(data.size() -1 == 4) { //Quad to triangle
                //Triangle 1
                positions.push_back(tempPositionVec[vertexIndices[0]]);
                positions.push_back(tempPositionVec[vertexIndices[1]]);
                positions.push_back(tempPositionVec[vertexIndices[2]]);
                //Triangle 2
                positions.push_back(tempPositionVec[vertexIndices[0]]);
                positions.push_back(tempPositionVec[vertexIndices[2]]);
                positions.push_back(tempPositionVec[vertexIndices[3]]);
                
                //Triangle1
                texCoords.push_back(tempTexCoordVec[texIndices[0]]);
                texCoords.push_back(tempTexCoordVec[texIndices[1]]);
                texCoords.push_back(tempTexCoordVec[texIndices[2]]);
                //Triangle2
                texCoords.push_back(tempTexCoordVec[texIndices[0]]);
                texCoords.push_back(tempTexCoordVec[texIndices[2]]);
                texCoords.push_back(tempTexCoordVec[texIndices[3]]);
            }
            
            //save the vertices finally
            for(int i = 0; i < positions.size() / 3; i++) {
                Vector a = positions[i*3];
                Vector b = positions[i*3+1];
                Vector c = positions[i*3+2];
                
                Vector normal = (b-a).cross(c-a);
                normal.normalize();
                
                vertices.push_back(Vertex(a, normal, texCoords[i*3].s, texCoords[i*3].t, curUsedMtl));
                vertices.push_back(Vertex(b, normal, texCoords[i*3+1].s, texCoords[i*3+1].t, curUsedMtl));
                vertices.push_back(Vertex(c, normal, texCoords[i*3+2].s, texCoords[i*3+2].t, curUsedMtl));
            }
        } else if (data[0].compare("mtllib") == 0) {
            if(mtlLib.compare("") != 0) {
                throw "Don't switch the mtllib in one obj file!";
            }
            mtlLib = data[1];
            
            ifstream fileMtl(mtlLib.c_str());
            string mtlLine;
            
            string curMtl = "";
            
            while (getline(fileMtl, mtlLine)) {
                trim(mtlLine);
                vector<string> mtlData = split(mtlLine, ' ');
                
                if(mtlData[0].compare("newmtl") == 0) { //mtl name
                    curMtl = mtlData[1];
                    mtlMap[curMtl] = Material();
                    mtlMap.at(curMtl).setName(mtlData[1]);
                } else if (mtlData[0].compare("Ns") == 0) { //spekularer Reflexionsexponent
                    mtlMap.at(curMtl).setSpecularExponent(stof(mtlData[1]));
                } else if (mtlData[0].compare("Ka") == 0) { //Ambiente Komponente
                    mtlMap.at(curMtl).setAmbientColor(Color(stof(mtlData[1]), stof(mtlData[2]), stof(mtlData[3])));
                } else if (mtlData[0].compare("Kd") == 0) { //Diffuse Reflexionskomponente
                    mtlMap.at(curMtl).setDiffuseColor(Color(stof(mtlData[1]), stof(mtlData[2]), stof(mtlData[3])));
                } else if (mtlData[0].compare("Ks") == 0) { //Spekulare Reflexionskomponente
                    mtlMap.at(curMtl).setSpecularColor(Color(stof(mtlData[1]), stof(mtlData[2]), stof(mtlData[3])));
                } else if (mtlData[0].compare("map_Kd") == 0) { //Textur für deffuse Komponente
                    mtlMap.at(curMtl).setDiffuseTexture(mtlData[1].c_str());
                }
            }
            
            fileMtl.close();
        } else if (data[0].compare("usemtl") == 0) {
            curUsedMtl = data[1];
        }
        //Ignore other unsupported line beginnings
    }
    
    findBounds();
    
    if(fit) { //Fit vertices to correct size
        float xRange = bounds.Max.X - bounds.Min.X;
        float yRange = bounds.Max.Y - bounds.Min.Y;
        float zRange = bounds.Max.Z - bounds.Min.Z;
        
        float maxRange = max(xRange, max(yRange, zRange));

        for(int i = 0; i < vertices.size(); i++) {
            vertices[i].Position.X = (vertices[i].Position.X / maxRange) * SCALE_FACTOR;
            vertices[i].Position.Y = (vertices[i].Position.Y / maxRange) * SCALE_FACTOR;
            vertices[i].Position.Z = (vertices[i].Position.Z / maxRange) * SCALE_FACTOR;
        }
        
        findBounds();
    }
    
    file.close();
    
    return vertices;
}

map<string, Material> ObjParser::getMtlMap() {
    return mtlMap;
}

BoundingBox ObjParser::getBoundingBox() {
    return bounds;
}

void ObjParser::findBounds() {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();
    
    for(int i = 0; i < tempPositionVec.size(); i++) {
        Vector toCheck = tempPositionVec[i];
        if(minX > toCheck.X) {
            minX = toCheck.X;
        }
        if(maxX < toCheck.X) {
            maxX = toCheck.X;
        }
        if(minY > toCheck.Y) {
            minY = toCheck.Y;
        }
        if(maxY < toCheck.Y) {
            maxY = toCheck.Y;
        }
        if(minZ > toCheck.Z) {
            minZ = toCheck.Z;
        }
        if(maxZ < toCheck.Z) {
            maxZ = toCheck.Z;
        }
    }
    Vector min = Vector(minX, minY, minZ);
    Vector max = Vector(maxX, maxY, maxZ);
    
    bounds = BoundingBox(min, max);
}

void ObjParser::trim(string& s) {
    while(s.compare(0,1," ")==0)
        s.erase(s.begin()); // remove leading whitespaces
    while(s.compare(0,1,"\t")==0) //remove leading tabs
        s.erase(s.begin());
    while(s.size()>0 && s.compare(s.size()-1,1," ")==0)
        s.erase(s.end()-1); // remove trailing whitespaces
}

vector<string> ObjParser::split(const string& s, char seperator) {
    vector<string> output;
    
    string::size_type prev_pos = 0, pos = 0;
    
    while((pos = s.find(seperator, pos)) != string::npos)
    {
        string substring( s.substr(prev_pos, pos-prev_pos) );
        
        if(!substring.empty())
            output.push_back(substring);
        
        prev_pos = ++pos;
    }
    
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    
    return output;
}