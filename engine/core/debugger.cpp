#include "debugger.hpp"
#include <iostream>
#include <format>

inline constexpr const GLchar* RED = "\x1b[31m";
inline constexpr const GLchar* ORANGE = "\x1b[38;5;214m";
inline constexpr const GLchar* YELLOW = "\x1b[33m";
inline constexpr const GLchar* BLUE = "\x1b[34m";
inline constexpr const GLchar* CLEAR = "\x1b[0m";

#define SET_COLOUR(colour,message) colour + message + CLEAR

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

const GLchar* Debugger::getSeverityColour(GLenum severity){
    switch(severity){
        case GL_DEBUG_SEVERITY_LOW:             return YELLOW;
        case GL_DEBUG_SEVERITY_MEDIUM:          return ORANGE;
        case GL_DEBUG_SEVERITY_HIGH:            return RED;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    return BLUE;
        default:                                return BLUE;
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

    std::string outputHeader = std::format("[LOG] <{}>",formatType(type));

    std::string output = std::format(
        R"(
| ID: {}
| SOURCE: {}
| SEVERITY: {}
| MESSAGE: {}

)",id,formatSource(source),formatSeverity(severity),message);

        std::cerr << SET_COLOUR(getSeverityColour(severity),outputHeader + output);
}