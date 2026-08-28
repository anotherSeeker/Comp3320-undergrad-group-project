#pragma once
#include <glad/gl.h>
#include <string>

#include <glm/glm.hpp>

class Shader{
    private:

    public:
    GLuint id;
    
    bool init(std::string vertexSource,std::string fragmentSource);
    void use();
    ~Shader();

    bool SetMat4(const char* uniformName,glm::mat4 value);


};
