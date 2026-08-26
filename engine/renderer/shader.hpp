#pragma once
#include <glad/gl.h>
#include <string>

class Shader{
    private:
    GLuint id;

    void CheckError(GLuint shader,const char* type);

    public:

    Shader(std::string vertexSource,std::string fragmentSource);

    void use();
    ~Shader();

};
