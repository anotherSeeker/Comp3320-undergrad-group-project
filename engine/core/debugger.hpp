#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Debugger{
    public:
    static void Init();

    private:
    static void Callback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, 
    const GLchar* message, 
    const void* userParam);

};
