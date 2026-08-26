#pragma once
#include <glad/gl.h>
#include <string>

class Shader{
    private:
    GLuint id;

    public:

    bool init(std::string vertexSource,std::string fragmentSource);
    void use();
    ~Shader();

};
