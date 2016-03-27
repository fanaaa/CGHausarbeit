//
//  Object.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 16.12.15.
//  Collaborator: Philipp Nardmann
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//


/*
 Klasse zum Einlesen, speichern und Darstellen eines Objektes aus einer .obj-Datei.
 Die Geometriedaten werden VBO gespeichert und Transformationen, über eigens verwaltete Transformationsmatrix realisiert.
 */
#define BUFFER_OFFSET(i) ((char *)NULL + (i)) // Kein Off-Set; alle Vektoren werden gezeichnet

using namespace std;
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

#include "Object.h"
#include <float.h>
#include <assert.h>
#include <math.h>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>

/*
 Hilfsklasse Vertex, zum Abspeichern der Vektoren und zugehörigen Normalen der Obj-Vertices.
 */
Vertex::Vertex(){}

Vertex::Vertex( const Vector& p, const Vector& n, float u, float v){
    Position = p;
    Normal = n;
    u = u;
    v = v;
}

Object::Object() : name(new char[256]), parentName(new char[256]), fileName(new char[256]), rotationAngle(0), rotationVector(Vector(0,0,0)), scaling(Vector(1,1,1)), translation(Vector(0,0,0)), indexSize(0), vertexSize(0), m_MaterialCount(0), m_pMaterials(new Material[20]){
    this->m_transMatrix.identity();
    this->isActiveObject = false;

}
/*
 Destruktor
 */
Object::~Object(){
}

/*
 Methode zum Parsen der .obj-Datei
 Parameter: Filename        - Dateiname der zu ladenden Obj-Datei
            translation     - Translationsvektor zum Darstellen der Ausgangsposition; eingelesen aus Szenendatei
            scaling         - Skalierungsvektor ""
            rotationVector  - Rotationsvektor ""
            rotationAngle   - Rotationsgrad ""
 
 */
bool Object::load(const char *Filename, Vector translation, Vector scaling, Vector rotationVector, float rotationAngle){
    
    this->translation = translation;
    this->scaling = scaling;
    this->rotationVector = rotationVector;
    this->rotationAngle = rotationAngle;
    
    struct Texcoord
    {
        float s,t;
    };
    
    struct Face
    {
        unsigned int pidx[3], tidx[3];
    };
    
    
    int result;
    Vector v;
    Vertex vertex;
    vector<Vertex> vertexBuffer;
    vector<unsigned int> indexBuffer;
    vector<Texcoord> vts;
    unsigned int pidx[4];
    unsigned int tidx[4];
    FILE * object = fopen(Filename, "r");
    
    if(object == NULL){
        cout << " Konnte Datei nicht oeffnen! - loadObject" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }else{
        while (1) {
            char lineHeader[256];
            result = fscanf(object, "%s", lineHeader);
            if(result == EOF){
                break;
            }else{
                if (strcmp(lineHeader, "v") == 0){
                    fscanf(object, "%a %a %a\n", &vertex.Position.X,&vertex.Position.Y,&vertex.Position.Z);
                    m_Vertices.push_back(vertex);
                }else if(strcmp(lineHeader, "mtllib") == 0){
                    char mtllib[30];
                    fscanf(object, "%s", mtllib);
                    this->loadMaterialFile(mtllib);
                }else if (strcmp(lineHeader, "vt") == 0){
                    Texcoord vt;
                    fscanf(object, "%a %a\n", &vt.s,&vt.t);
                    vts.push_back(vt);
                }else if (strcmp(lineHeader, "f") == 0){
                    Vertex vertex1;
                    Vertex vertex2;
                    Vertex vertex3;
                    Vertex vertex4;
                    Vector tmp1;
                    Vector tmp2;
                    Vector tmp3;
                    Vector tmp4;
                    int match = fscanf(object, "%d/%d %d/%d %d/%d %d/%d", &pidx[0],&tidx[0],&pidx[1],&tidx[1],&pidx[2],&tidx[2],&pidx[3],&tidx[3]);
                    
    
                    //Texturkoordinaten speichern
                    if(match == 6){
                        indexBuffer.push_back(pidx[0]-1);
                        indexBuffer.push_back(pidx[1]-1);
                        indexBuffer.push_back(pidx[2]-1);
                    }
                    if(match == 8){
                        indexBuffer.push_back(pidx[0]-1);
                        indexBuffer.push_back(pidx[1]-1);
                        indexBuffer.push_back(pidx[2]-1);
                        
                        indexBuffer.push_back(pidx[2]-1);
                        indexBuffer.push_back(pidx[3]-1);
                        indexBuffer.push_back(pidx[0]-1);
                        
                        
                    }
                }
                
            }
        }

        
        this->m_Indices = indexBuffer;
        
        //Normale berechnen
        Vector normal;
        for (int i = 0; i < indexBuffer.size() / 3; i++) {
            normal = (m_Vertices.at(indexBuffer[3*i+1]).Position - m_Vertices.at(indexBuffer[3*i]).Position).cross(m_Vertices.at(indexBuffer[3*i+2]).Position - m_Vertices.at(indexBuffer[3*i]).Position);
            
            normal.normalize();
            this->m_Vertices[indexBuffer[3*i]].Normal = normal;
            this->m_Vertices[indexBuffer[3*i+1]].Normal = normal;
            this->m_Vertices[indexBuffer[3*i+2]].Normal = normal;
            
        }
        
        
        
        //Objekte Grundskalieren
        this->createBoundingBox(m_Vertices);
        float scale = 5 / ((m_Box.Min - m_Box.Max).length());
        for(int i = 0; i < m_Vertices.size(); i++){
            this->m_Vertices[i].Position = m_Vertices[i].Position * scale;
        }
        
        
        this->indexSize = indexBuffer.size();
        
        //VBO erstellen
        glGenBuffers( 1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW );
        
        glGenBuffers( 1, &m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexBuffer.size(), &indexBuffer[0], GL_STATIC_DRAW);
        fclose(object);
        return true;
    }
    
}

/*
 Methode zum Übergeben der Transformationsmatrix an OpenGl.
 Objekt wird auf eigenen Matrix-Stack gepusht.
 */
void Object::vDraw(){
    glPushMatrix();
    glMultMatrixf(this->m_transMatrix);
    draw();
    glPopMatrix();
}

/*
 Methode zum Zeichnen der Objektdaten in OpenGL mit erstellten Bufferobjekten.
 */
void Object::draw(){
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    // setup position & normal pointers
    
    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
        float diff[4];
        float amb[4];
        float spec[4];
        for(int i = 0; i < m_MaterialCount; i++){
            diff[0] = m_pMaterials[i].getDiffuseColor().R;
            diff[1] = m_pMaterials[i].getDiffuseColor().G;
            diff[2] = m_pMaterials[i].getDiffuseColor().B;
            diff[3] = 0;
            
            amb[0] = m_pMaterials[i].getAmbientColor().R;
            amb[1] = m_pMaterials[i].getAmbientColor().G;
            amb[2] = m_pMaterials[i].getAmbientColor().B;
            amb[3] = 0;
            
            spec[0] = m_pMaterials[i].getSpecularColor().R;
            spec[1] = m_pMaterials[i].getSpecularColor().G;
            spec[2] = m_pMaterials[i].getSpecularColor().B;
            spec[3] = 0;
            
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
            glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
            glMateriali(GL_FRONT, GL_SHININESS, m_pMaterials[i].getSpecularExponent());
            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
        }
    if(this->isActiveObject){
        glDrawElements(GL_LINES, this->indexSize, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    }else {
        glDrawElements(GL_TRIANGLES, this->indexSize , GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    }
    //Normalwerte zurücksetzen
    float defDiff[4] = {1,1,1,1};
    float defAmb[4]  = {0.2f,0.2f,0.2f,1};
    float defSpec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, defDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, defSpec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, defAmb);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
   
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*
 Methode zum Erstellen der Boundingbox, die das Objekt in den Raum skaliert.
 Parameter: vertices - std::vector mit Vertices zur Ermittlung der Minima und Maxima.
 */
void Object::createBoundingBox(std::vector<Vertex> vertices){
    this->m_Box.Max.X = this->m_Box.Min.X = vertices[0].Position.X;
    this->m_Box.Max.Y = this->m_Box.Min.Y = vertices[0].Position.Y;
    this->m_Box.Max.Z = this->m_Box.Min.Z = vertices[0].Position.Z;
    
    for(int i = 0; i < vertices.size(); i++) {
        Vector vertex = vertices[i].Position;
        
        if(this->m_Box.Min.X < vertex.X) this->m_Box.Min.X = vertex.X;
        if(this->m_Box.Min.Y < vertex.Y) this->m_Box.Min.Y = vertex.Y;
        if(this->m_Box.Min.Z < vertex.Z) this->m_Box.Min.Z = vertex.Z;
        
        if(this->m_Box.Max.X > vertex.X) this->m_Box.Max.X = vertex.X;
        if(this->m_Box.Max.Y > vertex.Y) this->m_Box.Max.Y = vertex.Y;
        if(this->m_Box.Max.Z > vertex.Z) this->m_Box.Max.Z = vertex.Z;
    }
}

/*
 Methode zur Aktualisierung der Objektposition.
 */
void Object::update(){
    //Translate,Rotate,Scale
    Matrix TM;
    Matrix SM;
    Matrix RM;
    
    TM.identity();
    SM.identity();
    RM.identity();
    
    TM.translation(this->translation);
    SM.scale(this->scaling);
    RM.rotationAxis(this->rotationVector, this->rotationAngle);
    
    this->m_transMatrix =  TM * SM * RM;
}

/*
 Methode zur Überprüfung auf einen Mausklick des Objektes.
 */
bool Object::objectIsClicked(){
    for (int i = 0; i <  this->m_Indices.size()/ 3 ; i++) {
   
        Vector a = this->m_Vertices.at(this->m_Indices[3*i]).Position;
        Vector b = this->m_Vertices.at(this->m_Indices[3*i+1]).Position;
        Vector c = this->m_Vertices.at(this->m_Indices[3*i+2]).Position;
        
        //Transformationsmatrix anwenden
        a = this->m_transMatrix * a;
        b = this->m_transMatrix * b;
        c = this->m_transMatrix * c;

        float s;
        if (rayOrigin.triangleIntersection(worldDirection, a, b, c, s)) {
            return true;
        }
    }
    return false;
}

/*
 Hilfsmethode zum Laden des Materials
 Parameter: Filename - Name der Materialdatei
 */
bool Object::loadMaterialFile(const char* Filename){
    FILE* material = fopen(Filename,"r");
    int result;
    if(material == NULL){
        cout << " Konnte Datei nicht oeffnen! - loadMaterialFile" << endl;
        cout << getcwd(NULL, 0) << endl;
        perror("fopen");
        return false;
    }else{
        while (1) {
            char lineHeader[256];
            result = fscanf(material, "%s", lineHeader);
            if(result == EOF){
                break;
            }else{
                if(strcmp(lineHeader, "newmtl")){
                    loadMaterial(material);
                    m_MaterialCount++;
                }
            }
            fclose(material);
        }
        return true;
    }
}

/*
 Methode zum Einlesen der mtl-Datei und speichern des Materials im Objekt.
 Parameter: FILE* - Zeiger zur aktuell geöffneten Materialdatei.
 */
bool Object::loadMaterial(FILE*  materialfile){
    Material m;
    char lineHeader[256];
    int result;
    while (1) {
        char lineHeader[256];
        result = fscanf(materialfile, "%s", lineHeader);
        if(result == EOF){
            break;
        }else{
            if(strcmp(lineHeader, "newmtl") == 0){
                char name[128];
                fscanf(materialfile, "%s",name);
                m.setName(name);
            }else if (strcmp(lineHeader, "NS")== 0){
                float Ns;
                fscanf(materialfile, "%f",&Ns);
                m.setSpecularExponent(Ns);
            }else if(strcmp(lineHeader, "Ka") == 0){
                float Ka1,Ka2, Ka3;
                fscanf(materialfile, "%f %f %f", &Ka1,&Ka2,&Ka3);
                Color ka = Color(Ka1,Ka2,Ka3);
                m.setAmbientColor(ka);
            }else if(strcmp(lineHeader, "Kd")== 0){
                float Kd1, Kd2, Kd3;
                fscanf(materialfile, "%f %f %f",&Kd1,&Kd2,&Kd3);
                Color kd = Color(Kd1,Kd2,Kd3);
                m.setDiffuseColor(kd);
            }else if(strcmp(lineHeader, "Ks") == 0){
                float Ks1,Ks2,Ks3;
                fscanf(materialfile, "%f %f %f",&Ks1,&Ks2,&Ks3);
                Color ks = Color(Ks1,Ks2,Ks3);
                m.setSpecularColor(ks);
            }
        }
    }
    m_pMaterials[m_MaterialCount] = m;
    return true;
}



/*
 Set-Methoden
 */
void Object::setRotation(Vector &rotationVector, float rotationAngle){
    this->rotationVector = rotationVector;
    this->rotationAngle = rotationAngle;
}

void Object::setTranslation(Vector &translation){
    this->translation = translation;
}

void Object::setScaling(Vector& scaling){
    this->scaling = scaling;
}

void Object::setName(char *newName){
    this->name = newName;
}

void Object::setParentName(char *parentName){
    this->parentName = parentName;
}

void Object::setFileName(char *file){
    this->fileName = file;
}

void Object::setMatrix(Matrix newMatrix){
    this->m_transMatrix = newMatrix;
}

void Object::setActive(bool boolean){
    this->isActiveObject = boolean;
}

/*
 Get-Methoden
 */
Vector& Object::getTranslation(){
    return this->translation;
}

Vector& Object::getRotationVector(){
    return this->rotationVector;
}

float Object::getRotationAngle(){
    return this->rotationAngle;
}

Vector& Object::getScaling(){
    return this->scaling;
}

char* Object::getName(){
    return this->name;
}

char* Object::getParentName(){
    return this->parentName;
}

char* Object::getFileName(){
    return this->fileName;
}

Matrix Object::getMatrix(){
    return this->m_transMatrix;
}

char* Object::getVectorAsString(Vector& vector){
    char* string;
    
    return string;
}

bool Object::getActive(){
    return this->isActiveObject;
}
