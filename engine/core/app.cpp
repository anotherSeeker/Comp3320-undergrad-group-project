#include "./app.hpp"
#include "renderer/mesh.hpp"

#include <iostream>
#include <vector>

std::vector<vertex> vertices{
    glm::vec3()
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