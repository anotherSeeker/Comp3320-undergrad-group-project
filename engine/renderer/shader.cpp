#include "shader.hpp"

#include <cstring>
#include <iostream>

Shader::Shader(const char* vertexSource,const char* fragmentSource){
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexSource,nullptr);
    glCompileShader(vertexShader);
    CheckError(vertexShader,"VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentSource,nullptr);
    glCompileShader(fragmentShader);
    CheckError(fragmentShader,"FRAGMENT");

    id = glCreateProgram();
    glAttachShader(id,vertexShader);
    glAttachShader(id,fragmentShader);
    glLinkProgram(id);
    CheckError(id,"PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
    glUseProgram(id);
}

Shader::~Shader(){
    glDeleteProgram(id);
}

void Shader::CheckError(GLuint shader,const char* type){
    GLint hasCompiled;
    size_t logSize = 2 << 10;
    char infoLog[logSize]; 

    if(std::strcmp(type,"PROGRAM") == 0){
        glGetProgramiv(shader,GL_LINK_STATUS,&hasCompiled);
        if(hasCompiled == GL_TRUE) return;

        glGetProgramInfoLog(shader,logSize,nullptr,infoLog);
        std::cerr << "SHADER COMPILE FAILED: " << type << "\n" << infoLog << "\n";
    } else {
        glGetShaderiv(shader,GL_COMPILE_STATUS,&hasCompiled);
        if(hasCompiled == GL_TRUE) return;

        glGetShaderInfoLog(shader,logSize,nullptr,infoLog);
        std::cerr << "SHADER LINKING FAILED: " << type << "\n" << infoLog << "\n";
    }
}
