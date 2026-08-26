#include "debugger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

inline constexpr size_t KEY_WIDTH = 14;

inline constexpr std::string RED = "\x1b[31m";
inline constexpr std::string ORANGE = "\x1b[38;5;214m";
inline constexpr std::string YELLOW = "\x1b[93m";
inline constexpr std::string BLUE = "\x1b[34m";
inline constexpr std::string CLEAR = "\x1b[0m";

inline constexpr std::string BOLD = "\x1b[1m";
inline constexpr std::string UNBOLD = "\x1b[22m";

constexpr std::string SET_COLOUR(std::string colour,std::string message){
    return colour + message + CLEAR;
}

std::string padString(std::string text,size_t width){
    std::ostringstream stream;
    stream << std::left << std::setw(width) << text;
    
    return stream.str();
}

void Debugger::Init(){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

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

std::string Debugger::getSeverityColour(GLenum severity){
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

    std::cout << BOLD << "┍ LOG [" << formatType(type) << "]" << UNBOLD
                    << "\n│ " << BOLD << padString("ID:",KEY_WIDTH) << SET_COLOUR(YELLOW,std::to_string(id)) << UNBOLD
                    << "\n│ " << BOLD << padString("SOURCE:",KEY_WIDTH) << SET_COLOUR(BLUE,formatSource(source)) << UNBOLD
                    << "\n│ " << BOLD << padString("SEVERITY:",KEY_WIDTH) << SET_COLOUR(getSeverityColour(severity),formatSeverity(severity)) << UNBOLD
                    << "\n┕ " << BOLD << padString("MESSAGE:",KEY_WIDTH) << message << "\n" << UNBOLD;
}