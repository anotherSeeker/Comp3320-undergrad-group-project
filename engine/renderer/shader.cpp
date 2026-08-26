#include "shader.hpp"
#include "../core/debugger.hpp"

#include <cstring>
#include <iostream>

bool Shader::init(std::string vertexSource,std::string fragmentSource){

    const char* vertexRaw = vertexSource.data();
    const char* fragmentRaw = fragmentSource.data();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexRaw,nullptr);
    glCompileShader(vertexShader);
    if(Debugger::checkShaderError(vertexShader,"VERTEX")) return false;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentRaw,nullptr);
    glCompileShader(fragmentShader);
    if(Debugger::checkShaderError(fragmentShader,"FRAGMENT")) return false;

    id = glCreateProgram();
    glAttachShader(id,vertexShader);
    glAttachShader(id,fragmentShader);
    glLinkProgram(id);
    if(Debugger::checkShaderError(id,"PROGRAM")) return false;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::use(){
    glUseProgram(id);
}

Shader::~Shader(){
    glDeleteProgram(id);
}
