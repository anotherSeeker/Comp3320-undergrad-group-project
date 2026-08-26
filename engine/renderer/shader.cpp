#include "shader.hpp"
#include "../core/debugger.hpp"

#include <cstring>
#include <iostream>

Shader::Shader(std::string vertexSource,std::string fragmentSource){

    const char* vertexRaw = vertexSource.data();
    const char* fragmentRaw = fragmentSource.data();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexRaw,nullptr);
    glCompileShader(vertexShader);
    Debugger::checkShaderError(vertexShader,"VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentRaw,nullptr);
    glCompileShader(fragmentShader);
    Debugger::checkShaderError(fragmentShader,"FRAGMENT");

    id = glCreateProgram();
    glAttachShader(id,vertexShader);
    glAttachShader(id,fragmentShader);
    glLinkProgram(id);
    Debugger::checkShaderError(id,"PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
    glUseProgram(id);
}

Shader::~Shader(){
    glDeleteProgram(id);
}
