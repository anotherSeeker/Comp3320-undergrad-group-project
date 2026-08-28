#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer.hpp"

struct Window{
    GLFWwindow* windowObject;
    double lastX = 0;
    double lastY = 0;
};

class App{
    private:
    Window window;
    public:
    int32_t width;
    int32_t height;

    Renderer renderer;

    bool init(int32_t width,int32_t height,const char* title);
    void run();

    ~App();
};

