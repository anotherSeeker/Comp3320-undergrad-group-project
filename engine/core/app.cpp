#include "./app.hpp"
#include "renderer/mesh.hpp"

#include <iostream>
#include <vector>
#include <fstream>
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
    std::ifstream file{filePath};
    if(!file){
        std::cerr << "cannot load file\n";
        return "";
    }
    
    auto fileSize = file.seekg(0,std::ios::end).tellg();
    file.seekg(0);

    std::string fileContents{};
    fileContents.resize(fileSize);
    file.read(fileContents.data(),fileSize);

    return fileContents;
}

bool App::init(int32_t width,int32_t height,const char* title){
    std::cout << "init\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
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

    glViewport(0,0,width,height);

    return true;
}

void App::run(){
    std::cout << "running\n";

    Shader shader(loadFile("../assets/shaders/default.vert").data(),loadFile("../assets/shaders/default.frag").data());
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