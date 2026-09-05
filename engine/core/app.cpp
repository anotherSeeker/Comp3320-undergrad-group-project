#include "debugger.hpp"
#include "events/events.hpp"
#include "events/event_structs.hpp"
#include "events/event_enums.hpp"

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

    EventManager::createObserver("KeyPress",SK_EVENT_KEY_PRESS);
    EventManager::createObserver("KeyLifted",SK_EVENT_KEY_LIFTED);
    EventManager::createObserver("MousePress",SK_EVENT_MOUSE_PRESS);
    EventManager::createObserver("MouseLifted",SK_EVENT_MOUSE_LIFTED);
    EventManager::createObserver("MouseMove",SK_EVENT_MOUSE_MOVE);

    EventManager::createObserver("WindowResize",SK_EVENT_WINDOW_RESIZE);

    EventManager::createObserver("PreRender",SK_EVENT_PRERENDER);

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

    std::shared_ptr<Shader> shader = assetManager.loadShader("../assets/shaders/default.vert","../assets/shaders/default.frag");
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices,indices);

    glm::mat4 transform(1);
    transform = glm::translate(transform,glm::vec3(0,0,0));

    auto object = scene.createObject(mesh,shader,transform);

    float angle = glm::radians(0.0f);

    double previous = glfwGetTime();

    glClearColor(0.39,0.58,0.93,1.0);
    renderer.viewLookAt(glm::vec3(0,0,5),glm::vec3(0,0,0));

    while(!glfwWindowShouldClose(window.windowObject)){

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previous;
        previous = currentTime;

        scene.setRotation(object,glm::quat(glm::vec3(0,angle,0)));
        angle += glm::radians(50.0f * static_cast<float>(deltaTime));

        Debugger::incrementFrame();
        
        EventManager::dispatch("PreRender",static_cast<void*>(&deltaTime));
        if(window.width > 0 && window.height > 0){
            renderer.begin(window.width,window.height);

            scene.submitEntities(renderer);

            renderer.end();
        }
        

        glfwSwapBuffers(window.windowObject);
        glfwPollEvents();
        
    }
}

App::~App(){
    Debugger::print("finished");

    glfwDestroyWindow(window.windowObject);
    glfwTerminate();
}