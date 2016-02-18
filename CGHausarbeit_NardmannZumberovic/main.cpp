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
const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;
float angle = 0.0;
float zAxis = 0.0;
float xAxis = 0.0;
bool changes = false;
Room room = Room(10.0f, 10.0f, 10.0f);
char activeObject[56] = "vase";
Vector translation;
Vector rotationV;
float rotationA;
Vector scaling;
Object tmpObject;
Scene scene = Scene("test.osh");
const Vector g_LightPos = Vector( 0,4,0);
Camera g_Camera;
int g_MouseButton = 0;
int g_MouseState = 0;
ShaderProgram shader;

//Methodendeklaration
void DrawScene();
void SetupSettings();
void loadObjects();
void DrawGroundGrid();
void DrawObjects();
void DrawRoom();
void Draw();
void SetupShader();
void KeyboardCallback( unsigned char key, int x, int y);
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);



int main(int argc,char * argv[]) {
    
    glutInitWindowSize(g_WindowWidth , g_WindowHeight);
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Raumanwendung");
    
    SetupSettings();
    
    glutKeyboardFunc(KeyboardCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MouseMoveCallback);
    
    loadObjects();
    room.loadWall(shader,1.0f, 0.0f);
    room.loadFloor(shader,1.0f, 0.0f);
    
    
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

void DrawGroundGrid()
{
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for( unsigned int i=0; i<GridSegments+1; i++)
    {
        float itpos=GridOrigin + GridStep*(float)i;
        glVertex3f(itpos, 0, GridOrigin);
        glVertex3f(itpos, 0, GridOrigin+GridSize);
        
        glVertex3f(GridOrigin, 0, itpos);
        glVertex3f(GridOrigin+GridSize, 0, itpos);
        
    }
    glEnd();
    glEnable( GL_LIGHTING);
    
}

void loadObjects(){
    scene.parseFile();
    for(int i = 0; i < scene.objects.size(); i++){
        Object tmp = scene.objects.at(i);
        scene.objects.at(i).load(tmp.getFileName(), tmp.getTranslation(), tmp.getScaling(), tmp.getRotationVector() , tmp.getRotationAngle());
        tmpObject = tmp;
    }
}

void DrawObjects(){
    for(int i = 0; i < scene.objects.size(); i++){
        Object tmp = scene.objects.at(i);
        glPushMatrix();
        if(strcmp(tmp.getName(), activeObject)){
            glRotatef(angle, 0, 0, 1);
            glTranslatef(xAxis, 0, zAxis);
            tmp.draw();
        }else{
            glTranslatef(tmp.getTranslation().X, tmp.getTranslation().Y, tmp.getTranslation().Z);
            tmp.draw();
        }
        glPopMatrix();
    }
}

void DrawRoom(){
    room.drawRoom();
}

void SetupShader(){
    std::string err;
    
    //encoding is broken
    //-f PhongFragmentShader.glsl PhongVertexShader.glsl
    int argc = 5;
    char arg1[] = "-f";
    char arg2[] = "TextureCombinerFragment.glsl";//"PhongFragment.glsl";
    char arg3[] = "-v";
    char arg4[] = "TextureCombinerVertex.glsl";//"PhongFragment.glsl";
    
    ShaderProgram program;
    if(argc >= 3) {
        char* vertexFile = nullptr;
        char* fragmentFile = nullptr;
        if(strcmp(arg1, "-f") == 0) {
            fragmentFile = (char*) arg2;
        } else if(strcmp(arg1, "-v") == 0) {
            vertexFile = (char*) arg2;
        }
        
        if(argc == 5) {
            if(strcmp(arg3, "-f") == 0) {
                if (!fragmentFile) {
                    fragmentFile = (char*) arg4;
                }
            } else if(strcmp(arg3, "-v") == 0) {
                if (!vertexFile) {
                    vertexFile = (char*) arg4;
                }
            }
        }
        
        bool loadedFragment = false;
        if(fragmentFile) {
            if (program.loadFragmentShader(fragmentFile)) {
                loadedFragment = true;
            }
        }
        
        bool loadedVertex = false;
        if(vertexFile) {
            if(program.loadVertexShader(vertexFile)) {
                loadedVertex = true;
            }
        }
        
        if(loadedFragment || loadedVertex) {
            if(!program.compile(&err)) {
                std::cout << err << std::endl;
            }
        }
        
    }

}

void KeyboardCallback( unsigned char key, int x, int y){
    if (key == 'l'){
        angle += .5;
    }
    if (key == 't'){
        angle -= .5;
    }
    if (key == 'c') {
        //translation, rotation, scaling in Object speichern
        if(changes){
            Vector transformation = Vector(xAxis,0,zAxis);
            for (int i = 0; i < scene.objects.size(); i++) {
                if (strcmp(activeObject, scene.objects.at(i).getName())== 0) {
                        scene.objects.at(i).setTranslation(transformation);
                    }
                }
        }
        changes = false;
        angle = 0.0;
        zAxis = 0.0;
        xAxis = 0.0;
        if(strcmp(activeObject, "vase") == 0){
            strcpy(activeObject, "tisch");
            cout << "in Vase" << endl;
        }else{
            strcpy(activeObject, "vase");
            cout << "in tisch" << endl;
        }
    }
    if (key == 'w') {
        changes = true;
        zAxis += .5;
    }
    if (key == 's') {
        changes = true;
        zAxis -= .5;
    }
    if (key == 'a') {
        changes = true;
        xAxis += .5;
    }
    if (key == 'd') {
        changes = true;
        xAxis -= .5;
    }
}

void MouseCallback(int Button, int State, int x, int y)
{
    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);
}

void MouseMoveCallback( int x, int y)
{
    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

