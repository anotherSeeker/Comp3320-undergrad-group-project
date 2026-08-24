#pragma once
#include <glad/gl.h>

class Shader{
    private:
    GLuint id;

    void CheckError(GLuint shader,const char* type);

    public:

    Shader(const char* vertexSource,const char* fragmentSource);

    void use();
    ~Shader();

};
