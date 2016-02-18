//
//  ShaderProgram.hpp
//  RoomTest
//
//  Created by Fana Zumberovic on 22.01.16.
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <stdio.h>
#include <string>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "color.h"
#include "Matrix.h"
#include <fstream>

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    bool load(const char* VertexShader, const char* FragmentShader);
    bool loadVertexShader(const char* VertexShader);
    bool loadFragmentShader(const char* FragmentShader);
    bool compile(std::string* CompileErrors=NULL);
    
    GLint getParameterID(const char* ParameterName) const;
    
    void setParameter(GLint ID, float Param);
    void setParameter(GLint ID, int Param);
    void setParameter(GLint ID, const Vector& Param);
    void setParameter(GLint ID, const Color& Param);
    void setParameter(GLint ID, const Matrix& Param);
    
    void activate() const;
    void deactivate() const;
private:
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    GLuint m_ShaderProgram;
    
    void unloadShader(GLchar** ShaderSource);
    bool loadShader(const char* filename, GLchar** ShaderSource, unsigned long* len);
    unsigned long getFileLength(std::ifstream& file);
    
};

#endif /* ShaderProgram_hpp */
