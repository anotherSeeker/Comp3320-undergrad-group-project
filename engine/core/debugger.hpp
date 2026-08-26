#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

class Debugger{
    public:
    static void init();
    static void incrementFrame();

    static void print(std::string message);
    static bool checkShaderError(GLuint shader,const char* type);

    private:

    static size_t frames;

    static const GLchar* formatSource(GLenum source);
    static const GLchar* formatType(GLenum type);
    static const GLchar* formatSeverity(GLenum severity);

    static std::string getSeverityColour(GLenum severity);

    static void callback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, 
    const GLchar* message, 
    const void* userParam);

    
};
