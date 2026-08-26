#include "debugger.hpp"

#include "./app.hpp"
#include "renderer/mesh.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

std::vector<vertex> vertices{
    vertex{glm::vec3{-0.5, -0.5, 0}},
    vertex{glm::vec3{ 0.0, 0.5, 0}},
    vertex{glm::vec3{ 0.5, -0.5, 0}},
};

std::vector<GLuint> indices{
    0,1,2
};

std::string loadFile(std::string filePath){
    std::ifstream file;
    file.open(filePath);

    if(!file.is_open()){
        std::cerr << "cannot load file\n";
        return "";
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

void GLAPIENTRY debugCallback(
    GLenum source, 
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length, const 
    GLchar* message, 
    const void* userParam){

    std::cerr << "OpenGL error: " << message << "\n";
}


bool App::init(int32_t width,int32_t height,const char* title){
    std::cout << "init\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    
    window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!window){
        std::cerr << "failed to create window\n";
        return false;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress)){
        std::cerr << "failed to load openGL pointers with glad\n";
        return false;
    };

    glViewport(0,0,width,height);

    Debugger::Init();

    return true;
}

void App::run(){
    std::cout << "running\n";

    Shader shader(loadFile("../assets/shaders/default.vert"),loadFile("../assets/shaders/default.frag"));
    Mesh mesh(vertices,indices);

    glClearColor(0.39,0.58,0.93,1.0);

    while(!glfwWindowShouldClose(window)){
        

        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawMesh(mesh,shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
}

App::~App(){
    std::cout << "finished\n";

    glfwDestroyWindow(window);
    glfwTerminate();
}