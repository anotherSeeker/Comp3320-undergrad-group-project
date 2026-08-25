#include "./app.hpp"
#include "renderer/mesh.hpp"

#include <iostream>
#include <vector>
#include <fstream>
// #include <string>

std::vector<vertex> vertices{
    vertex{glm::vec3{-0.5f, 0.0f, 0.0f}},
    vertex{glm::vec3{ 0.0f, 0.5f, 0.0f}},
    vertex{glm::vec3{ 0.5f, 0.0f, 0.0f}},
};

std::vector<GLuint> indices{
    0,1,2
};

std::string loadFile(std::string filePath){
    std::ifstream file(filePath);
    if(!file){
        std::cerr << "cannot load file\n";
        return "";
    }

    auto fileSize = file.seekg(0,std::ios::end).tellg();

    //build succeeds .exe launches
    file.seekg(0, std::ios::beg);
    //build succeeeds .exe fails to launch with error "The procedure entry point _ZNSi5seekgESt4fposliE could not be located in the dynamic link library C:/3320-group-project\Comp3320-undergrad-group-project\sandbox\libsunked_castle.dll"
    //file.seekg(0);

    std::string fileContents{};
    fileContents.resize(fileSize);
    file.read(fileContents.data(), fileSize);

    return fileContents;
}

bool App::init(int32_t width,int32_t height,const char* title){
    std::cout << "init\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
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

    return true;
}

void App::run(){
    std::cout << "running\n";

    Mesh mesh(vertices,indices);

    glClearColor(0.39,0.58,0.93,1.0);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

App::~App(){
    std::cout << "finished\n";

    glfwDestroyWindow(window);
    glfwTerminate();
}