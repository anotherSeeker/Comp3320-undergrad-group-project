#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer.hpp"

class App{
    private:
    GLFWwindow* window;
    Renderer renderer;
    public:
    bool init(int32_t width,int32_t height,const char* title);
    void run();

    ~App();
};

