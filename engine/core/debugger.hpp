#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

class Debugger{
    public:
    static void Init();
    static void incrementFrame();

    private:

    static size_t frames;

    static const GLchar* formatSource(GLenum source);
    static const GLchar* formatType(GLenum type);
    static const GLchar* formatSeverity(GLenum severity);

    static std::string getSeverityColour(GLenum severity);

    static void Callback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, 
    const GLchar* message, 
    const void* userParam);

    
};
