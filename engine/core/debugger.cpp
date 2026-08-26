#include "debugger.hpp"
#include <iostream>
#include <format>

void Debugger::Init(){
    glfwWindowHint(GLFW_CONTEXT_DEBUG,GL_TRUE);
    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(Debugger::Callback,nullptr);
}

const GLchar* Debugger::formatSource(GLenum source){
    switch(source){
        case GL_DEBUG_SOURCE_API:             return "API";
        case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
        default:                              return "UNKNOWN";
    }
}

const GLchar* Debugger::formatType(GLenum type){
    switch(type){
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_ERROR:               return "ERROR";
        case GL_DEBUG_TYPE_MARKER:              return "MARKER";
        case GL_DEBUG_TYPE_OTHER:               return "OTHER";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
        case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
        case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
        default:                                return "UNKNOWN";
    }
}

const GLchar* Debugger::formatSeverity(GLenum severity){
    switch(severity){
        case GL_DEBUG_SEVERITY_LOW:             return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM:          return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH:            return "HIGH";
        case GL_DEBUG_SEVERITY_NOTIFICATION:    return "NOTIFICATION";
        default:                                return "UNKNOWN";
    }
}


void Debugger::Callback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, 
    const GLchar* message, 
    const void* userParam
){

    std::string output = std::format(
        R"([LOG] <{}>
| ID: {}
| SOURCE: {}
| SEVERITY: {}
| MESSAGE: {}

)",formatType(type),id,formatSource(source),formatSeverity(severity),message);

        std::cerr << output;
}