#include "debugger.hpp"
#include <iostream>

void Debugger::Init(){
    glfwWindowHint(GLFW_CONTEXT_DEBUG,GL_TRUE);
    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(Debugger::Callback,nullptr);
}

void Debugger::Callback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, const 
    GLchar* message, 
    const void* userParam){

    std::cerr << "OpenGL error: " << message << "\n";
}