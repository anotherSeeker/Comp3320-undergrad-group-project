#pragma once
#include <glad/gl.h>

class Shader{
    private:
    GLuint id;

    public:

    Shader(const char* vertexSource,const char* fragmentSource);

    void use();
    ~Shader();

};
