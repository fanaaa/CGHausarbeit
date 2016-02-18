//
//  ShaderProgram.cpp
//  RoomTest
//
//  Created by Fana Zumberovic on 22.01.16.
//  Copyright © 2016 HochschuleOsnabrueck. All rights reserved.
//

#include "ShaderProgram.h"


ShaderProgram::ShaderProgram() {
    
}

ShaderProgram::~ShaderProgram() {
    
}

/**
 * lädt die übergebene Vertex- und Fragment-Shader-Datei. Hierfür nutzt die
 * Methode intern loadVertexShader() und loadFragmentShader().
 */
bool ShaderProgram::load(const char* VertexShader, const char* FragmentShader) {
    return loadVertexShader(VertexShader) && loadFragmentShader(FragmentShader);
}

bool ShaderProgram::loadVertexShader(const char* VertexShader) {
    m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    bool toRet = false;
    
    GLchar *source;
    unsigned long length;
    if(loadShader(VertexShader, &source, &length)) {
        const char * src = source;
        glShaderSource(m_VertexShader, 1, &src, NULL); //length isn't allowed here ?!
        toRet = true;
    }
    
    
    return toRet;
}

bool ShaderProgram::loadFragmentShader(const char* FragmentShader) {
    m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    bool toRet = false;
    
    GLchar *source;
    unsigned long length;
    if(loadShader(FragmentShader, &source, &length)) {
        const char * src = source;
        glShaderSource(m_FragmentShader, 1, &src, NULL); //length isn't allowed here ?!
        toRet = true;
    }
    
    
    return toRet;
}

/**
 * kompiliert & linkt die Shader. Lässt sich ein Shader nicht kompilieren,
 * soll die Routine false zurückliefern und die Fehlermeldungen des Compilers in
 * CompileErrors schreiben (sofern CompileErrors!=NULL).
 */
bool ShaderProgram::compile(std::string* CompileErrors) {
    bool toRet = false;
    GLint compiled = 0;
    
    if (m_VertexShader) {
        glCompileShader(m_VertexShader);
        glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_FALSE)
        {
            toRet = true;
        } else {
            GLint blen = 0;
            GLsizei slen = 0;
            
            toRet = false;
            
            glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH , &blen);
            if (blen > 1)
            {
                GLchar* compiler_log = (GLchar*)malloc(blen);
                glGetShaderInfoLog(m_VertexShader, blen, &slen, compiler_log);
                std::string err(compiler_log);
                CompileErrors = &err;
                free (compiler_log);
            }
        }
    } else {
        toRet = true;
    }
    if(toRet) { //only continue if everything worked
        if(m_FragmentShader) {
            glCompileShader(m_FragmentShader);
            glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &compiled);
            if (compiled != GL_FALSE)
            {
                toRet = true;
            } else {
                GLint blen = 0;
                GLsizei slen = 0;
                
                toRet = false;
                
                glGetShaderiv(m_FragmentShader, GL_INFO_LOG_LENGTH , &blen);
                if (blen > 1)
                {
                    GLchar* compiler_log = (GLchar*)malloc(blen);
                    glGetShaderInfoLog(m_FragmentShader, blen, &slen, compiler_log);
                    std::string err(compiler_log);
                    CompileErrors = &err;
                    free (compiler_log);
                }
            }
        }
    }
    
    if(toRet) {
        m_ShaderProgram = glCreateProgram();
        
        if(m_VertexShader) {
            glAttachShader(m_ShaderProgram, m_VertexShader);
        }
        if(m_FragmentShader) {
            glAttachShader(m_ShaderProgram, m_FragmentShader);
        }
        
        glLinkProgram(m_ShaderProgram);
        GLint linked = 0;
        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &linked);
        if (linked != GL_FALSE)
        {
            toRet = true;
            if(m_VertexShader) {
                glDeleteShader(m_VertexShader);
            }
            if(m_FragmentShader) {
                glDeleteShader(m_FragmentShader);
            }
        } else {
            toRet = false;
            
            GLint blen = 0;
            GLsizei slen = 0;
            
            glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH , &blen);
            if (blen > 1)
            {
                GLchar* compiler_log = (GLchar*)malloc(blen);
                glGetProgramInfoLog(m_ShaderProgram, blen, &slen, compiler_log);
                std::string err(compiler_log);
                CompileErrors = &err;
                free (compiler_log);
            }
            
            glDeleteProgram(m_ShaderProgram);
        }
        
        //        if(m_VertexShader) {
        //            glDetachShader(m_ShaderProgram, m_VertexShader);
        //        }
        //        if(m_FragmentShader) {
        //            glDetachShader(m_ShaderProgram, m_FragmentShader);
        //        }
    }
    
    return toRet;
}

/**
 * liefert die ID (Location) einer Uniform-Variable im Shader
 * zurück.
 */
GLint ShaderProgram::getParameterID(const char* ParameterName) const {
    GLint Loc = glGetUniformLocation(m_ShaderProgram,ParameterName);
    return Loc;
}

/**
 * erlaubt es, den Wert einer Uniform-Variable zu setzen, hier
 * werden die Typen float, int, Vector, Color & Matrix unterschieden.
 */
void ShaderProgram::setParameter(GLint ID, float Param) {
    glUniform1f(ID,Param);
}

void ShaderProgram::setParameter(GLint ID, int Param) {
    glUniform1i(ID,Param);
}

void ShaderProgram::setParameter(GLint ID, const Vector& Param) {
    glUniform3f(ID,Param.X,Param.Y,Param.Z);
}

void ShaderProgram::setParameter(GLint ID, const Color& Param) {
    glUniform3f(ID,Param.R,Param.G,Param.B);
}

void ShaderProgram::setParameter(GLint ID, const Matrix& Param) {
    glUniformMatrix4fv(ID,1,GL_FALSE,Param.m);
}

/**
 * activate() und deactivate() aktivieren bzw. deaktivieren das Shader-Programm.
 * Nach dem Aufruf von activate() können die Modelldaten gezeichnet werden, die
 * den Shader verwenden sollen.
 */
void ShaderProgram::activate() const {
    glUseProgram(m_ShaderProgram);
}

void ShaderProgram::deactivate() const {
    glUseProgram(0);
}

void ShaderProgram::unloadShader(GLchar** ShaderSource) {
    if(*ShaderSource != 0) {
        delete []  *ShaderSource;
    }
    *ShaderSource = 0;
}

unsigned long ShaderProgram::getFileLength(std::ifstream& file) {
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0,std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);
    
    return len;
}

bool ShaderProgram::loadShader(const char* filename, GLchar** ShaderSource, unsigned long* len) {
    std::ifstream file;
    file.open(filename, std::ios::in);
    if(!file) return false;
    
    unsigned long length = getFileLength(file);
    
    len = &length;
    
    if (len==0) return false;   // Empty File
    
    *ShaderSource = (GLchar*) new char[length+1];
    if (*ShaderSource == 0) return false;   // can't reserve memory
    
    // safety 0 terminate
    (*ShaderSource)[length] = 0;
    
    unsigned int i=0;
    while (file.good())
    {
        (*ShaderSource)[i] = file.get();
        if (!file.eof())
            i++;
    }
    
    (*ShaderSource)[i] = 0;  // 0-terminate
    
    file.close();
    
    return true; // No Error
}