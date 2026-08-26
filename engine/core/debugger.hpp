#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Debugger{
    public:
    static void Init();

    private:

    static const GLchar* formatSource(GLenum source);
    static const GLchar* formatType(GLenum type);
    static const GLchar* formatSeverity(GLenum severity);

    static const GLchar* getSeverityColour(GLenum severity);

    static void Callback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, 
    const GLchar* message, 
    const void* userParam);

    
};
