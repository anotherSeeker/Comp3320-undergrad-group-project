#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

class App{
    private:
    GLFWwindow* window;
    public:
    bool init(int32_t width,int32_t height,const char* title);
    void run();

    ~App();
};

