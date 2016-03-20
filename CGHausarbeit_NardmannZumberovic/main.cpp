//
//  main.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 14.12.15.
//  Collaborator: Philipp Nardmann
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <iostream>
#include "Scene.h"
#include "Camera.h"
#include "Room.h"
#include "ShaderProgram.h"
#include <string.h>
#include <stdio.h>

//Verwendete Objekte und Konstanten
const unsigned int g_WindowWidth= 1024;
const unsigned int g_WindowHeight= 768;
bool rotation = false;
float angle = 0.0;
Room room = Room(15.0f, 15.0f, 15.0f);
Vector translation;
Vector rotationV;
float rotationA;
Vector scaling;
float scalingFactor;
Scene scene = Scene("test.osh");
const Vector g_LightPos = Vector( 0,4,0);
Camera g_Camera;
int g_MouseButton = 0;
int g_MouseState = 0;
ShaderProgram shader;
bool camera = false;
Vector rayOrigin;
Vector worldDirection;

//Methodendeklaration
void DrawScene();
void SetupSettings();
void loadObjects();
void DrawGroundGrid();
void DrawObjects();
void DrawRoom();
void Draw();
void SetupShader();
void resetVectors();
void updateActiveObject(int i);
void coutMatrix(Matrix &matrix);
void KeyboardCallback( unsigned char key, int x, int y);
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void SpecialCallback(int key,int x,int y);
void initObjects();
void coutMatrix(Matrix &matrix);
void calcMouseRay(int x,int y);



int main(int argc,char * argv[]) {
    glutInitWindowSize(g_WindowWidth , g_WindowHeight);
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Raumanwendung");
    
    SetupSettings();
    
    glutKeyboardFunc(KeyboardCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MouseMoveCallback);
    glutSpecialFunc(SpecialCallback);

    
    loadObjects();
    scene.activeObject = "Puppe";
    initObjects();
    resetVectors();
    
    scene.room.loadWall();
    scene.room.loadFloor();
    
    
    glutDisplayFunc(DrawScene);
    
    glutMainLoop();
    

    return 0;
}

void DrawScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    g_Camera.apply();
    
    DrawRoom();
    DrawObjects();
    
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void SetupSettings(){
        glClearColor(0, 0, 0, 255);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    
        glCullFace(GL_BACK);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(65, (double)g_WindowWidth/(double)g_WindowHeight, 0.045f, 1000.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Setup Light Color
        GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
        GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        
        // Setup default material
        float diff[4] = {1,1,1,1};
        float amb[4]  = {0.2f,0.2f,0.2f,1};
        float spec[4] = {0.5f,0.5f,0.5f,1};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMateriali(GL_FRONT, GL_SHININESS, 30);
        glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
        
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    
    
}

//Raum und Objekte laden
void loadObjects(){
    scene.parseFile();
    for(Object& tmp : scene.objects){
         tmp.load(tmp.getFileName(), tmp.getTranslation(), tmp.getScaling(), tmp.getRotationVector() , tmp.getRotationAngle());
    }
}

//Objekte darstellen
void DrawObjects(){
    int i = 0;
    for(Object& tmp : scene.objects){
        tmp.setActive(false);
        if(!strcmp(tmp.getName(), scene.activeObject)){
            updateActiveObject(i);
            tmp.setActive(true);
        }
        glPushMatrix();
        glMultMatrixf(tmp.getMatrix());
        tmp.draw();
        glPopMatrix();
        i++;
    }
}

//Raum darstellen
void DrawRoom(){
    scene.room.drawRoom();
}


void KeyboardCallback( unsigned char key, int x, int y){
    //Rotation
    if (key == 'l'){
        angle += .5;
    }
    if (key == 't'){
        angle -= .5;
    }
    //Translation
    if (key == 'w') {
        translation.Z += .5;
    }
    if (key == 's') {
        translation.Z -= .5;
    }
    if (key == 'a') {
        translation.X += .5;
    }
    if (key == 'd') {
        translation.X -= .5;
    }
    //Scaling
    if (key == '+') {
        scalingFactor += .1;
    }
    if (key == '-') {
        scalingFactor -= .1;
    }
    //Rotation
    if (key == 'r') {
        if (rotation) {
            rotation = false;
        }else{
            rotation = true;
        }
    }
    if(key == 'y'){
        rotationV  = Vector(0,1,0);
        rotationA = 0;
    }
    //Datei speichern
    if (key == 13) {
        scene.saveFile();
    }
}

void SpecialCallback(int key,int x,int y){
    if (key == GLUT_KEY_UP) {
        translation.Y += .5;
    }
    if (key == GLUT_KEY_DOWN) {
        translation.Y -= .5;
    }
    if (key == GLUT_KEY_RIGHT) {
        rotationA += .2;
    }
    if (key == GLUT_KEY_LEFT) {
        rotationA -= .1;
    }
}

void coutMatrix(Matrix &matrix){
    for (int i = 0; i < 16; i++) {
        cout << matrix.m[i] << endl;
    }
}

//Funktion zum Setzen der Vektoren des aktiven Objektes, um diese transformieren zu können
void resetVectors(){
    translation = scene.getObjectByName(scene.activeObject).getTranslation();

    scalingFactor = 1;
    
    scaling = scene.getObjectByName(scene.activeObject).getScaling();
    
    rotationV = scene.getObjectByName(scene.activeObject).getRotationVector();
    rotationA = scene.getObjectByName(scene.activeObject).getRotationAngle();
    
}

void MouseCallback(int Button, int State, int x, int y)
{
    g_MouseButton = Button;
    g_MouseState = State;
    if(State == GLUT_DOWN && camera ){
        g_Camera.mouseInput(x,y,Button,State);
    }
    //Auswahl des aktiven Objektes
    if (State == GLUT_UP) {
        calcMouseRay(x, y);
        scene.setActiveObject();
        resetVectors();
    }
}

void MouseMoveCallback( int x, int y)
{
    if (rotation){
        rotationA =  x * .01;
    }
    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

//Transformationsmatrix aktualisieren
void updateActiveObject(int i){
    Vector newScaling = Vector(scaling.X * scalingFactor,scaling.Y * scalingFactor, scaling.Z* scalingFactor);
    scene.objects.at(i).setTranslation(translation);
    scene.objects.at(i).setScaling(newScaling);
    scene.objects.at(i).setRotation(rotationV, rotationA);
    scene.objects.at(i).update();
}

//Objekte an die richtige Stelle positionieren
void initObjects(){
    for(Object& tmp : scene.objects){
        tmp.update();
    }
}

//Mausposition in Vektor umwandeln (gibt nur einen Punkt der x,y Ebene zurück)
void calcMouseRay(int mouseX, int mouseY) {
    // Umrechnung des Mauszeigers in den normalisierten Bildraum
    float viewportX = (2.0f/g_WindowWidth) * (mouseX - (g_WindowWidth*0.5));
    float viewportY = (2.0f/g_WindowHeight) * (mouseY - (g_WindowHeight*0.5));
    
    // Richtungsvektor im Kameraraum bestimmen
    Matrix invertProjection = g_Camera.getProjectionMatrix();
    invertProjection.invert();
    Vector camDirection = invertProjection.transformVec4x4(Vector(viewportX, -viewportY, 0));
    camDirection.normalize();
    
    // Richtungsvektor in Welt-Koordinaten umrechnen
    Matrix invertViewMatrix = g_Camera.getViewMatrix();
    invertViewMatrix.invert();
    worldDirection = invertViewMatrix.transformVec3x3(camDirection);
    rayOrigin = invertViewMatrix.translation();
}


