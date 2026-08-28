#include "debugger.hpp"
#include "events/events.hpp"

#include "events/event_structs.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "./app.hpp"
#include "renderer/mesh.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <format>

std::vector<vertex> vertices{
    vertex{glm::vec3{-0.5, -0.5, -0.5}},
    vertex{glm::vec3{ 0.5, -0.5, -0.5}},
    vertex{glm::vec3{-0.5, -0.5,  0.5}},
    vertex{glm::vec3{ 0.5, -0.5,  0.5}},
    vertex{glm::vec3{ 0.0,  0.5,  0.0}},
};

std::vector<GLuint> indices{
    0,2,1,
    1,2,3,
    0,1,4,
    1,3,4,
    3,2,4,
    2,0,4
};

std::string loadFile(std::string filePath){
    std::ifstream file;
    file.open(filePath);

    if(!file.is_open()){
        Debugger::print(std::format("cannot loadfile {}",filePath));
        return "";
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (action)
    {
    case GLFW_PRESS:
        EventManager::dispatch("KeyPress",static_cast<void*>(&key));
        break;
    case GLFW_RELEASE:
        EventManager::dispatch("KeyLifted",static_cast<void*>(&key));
        break;
    
    default:
        break;
    }
}

void mouseClickCallback(GLFWwindow* windowObject, int button, int action, int mods){
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(windowObject));

    if(action == GLFW_PRESS){
        
        EventManager::dispatch("MousePress",static_cast<void*>(&button));
    } else if(action == GLFW_RELEASE) {
        
        EventManager::dispatch("MouseLifted",static_cast<void*>(&button));
    }

}

void mouseMoveCallback(GLFWwindow* windowObject,double positionX, double positionY){
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(windowObject));

    SK_MOUSE_MOVE_EVENT event{
        .mouseX = positionX,
        .mouseY = positionY,
        .deltaX = positionX - window->lastX,
        .deltaY = positionY - window->lastY
    };

    window->lastX = positionX;
    window->lastY = positionY;

    EventManager::dispatch("MouseMove",static_cast<void*>(&event));
}

void windowResizeCallback(GLFWwindow* windowObject,int width,int height){
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(windowObject));

    window->width = width;
    window->height = height;

    SK_WINDOW_RESIZE_EVENT event{
        .width = width,
        .height = height,
    };

    EventManager::dispatch("WindowResize",static_cast<void*>(&event));
}

bool App::init(int32_t width,int32_t height,const char* title){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* windowObject = glfwCreateWindow(width,height,title,nullptr,nullptr);

    if(!windowObject){
        std::cerr << "failed to create window\n";
        return false;
    }

    glfwMakeContextCurrent(windowObject);
    if(!gladLoadGL(glfwGetProcAddress)){
        std::cerr << "failed to load openGL pointers with glad\n";
        return false;
    };

    Debugger::init();
    Debugger::print("init");
    glViewport(0,0,width,height);

    EventManager::createObserver("KeyPress",1);
    EventManager::createObserver("KeyLifted",2);
    EventManager::createObserver("MousePress",3);
    EventManager::createObserver("MouseLifted",4);
    EventManager::createObserver("MouseMove",5);

    EventManager::createObserver("WindowResize",6);

    EventManager::createObserver("PreRender",7);

    window = {
        .windowObject = windowObject,
        .lastX = 0,
        .lastY = 0,
        .width = width,
        .height = height,
    };

    glfwSetWindowUserPointer(window.windowObject,&window);

    glfwSetKeyCallback(windowObject,keyCallback);
    glfwSetMouseButtonCallback(windowObject,mouseClickCallback);
    glfwSetCursorPosCallback(windowObject,mouseMoveCallback);
    glfwSetFramebufferSizeCallback(windowObject,windowResizeCallback);

    return true;
}

void App::run(){
    Debugger::print("running");

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices,indices);

    if(!shader->init(loadFile("../assets/shaders/default.vert"),loadFile("../assets/shaders/default.frag"))){
        return;
    }

    glm::mat4 transform(1);
    transform = glm::translate(transform,glm::vec3(0,0,0));

    double previous = glfwGetTime();

    glClearColor(0.39,0.58,0.93,1.0);

    renderer.viewLookAt(glm::vec3(0,0,5),glm::vec3(0,0,0));

    while(!glfwWindowShouldClose(window.windowObject)){

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previous;
        transform = glm::rotate(transform,glm::radians(50.0f * static_cast<float>(deltaTime)),glm::vec3(0.0f,1.0f,0.0f));

        previous = currentTime;

        Debugger::incrementFrame();
        
        EventManager::dispatch("PreRender",static_cast<void*>(&deltaTime));

        renderer.begin(window.width,window.height);

        renderer.submit(mesh,shader,transform);

        renderer.end();

        glfwSwapBuffers(window.windowObject);
        glfwPollEvents();
        
    }
}

App::~App(){
    Debugger::print("finished");

    glfwDestroyWindow(window.windowObject);
    glfwTerminate();
}