#include "shader.hpp"

Shader::Shader(const char* vertexSource,const char* fragmentSource){
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexSource,nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentSource,nullptr);
    glCompileShader(fragmentShader);

    id = glCreateProgram();
    glAttachShader(id,vertexShader);
    glAttachShader(id,fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
    glUseProgram(id);
}

Shader::~Shader(){
    glDeleteProgram(id);
}
