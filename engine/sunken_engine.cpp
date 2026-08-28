#include <sunken_engine.hpp>
#include <iostream>
#include <format>

#include "./core/debugger.hpp"

#include "./core/app.hpp"
#include "./core/events/events.hpp"

App app;

bool skInit(){
    if(!app.init(800,600,"App")){
        std::cerr << "failed to open app\n";
        return false;
    }
    return true;
}

void skRun(){
    app.run();
}

void skEventCallback(void (*eventCallback)(int,int,void*)){
    EventManager::eventCallback = eventCallback;
}

void skLog(const char* message){
    Debugger::print(message);
}

void skListen(const char* eventName,int callback){
    
    std::string eventNameString = static_cast<std::string>(eventName);

    if(!EventManager::listen(eventNameString,callback))
        Debugger::print(std::format("Failed to connect to observer {}",eventNameString));
}

void skMoveView(float x,float y,float z){
    app.renderer.moveView(glm::vec3(x,y,z));
}

void skRotateView(float x,float y,float z){
    app.renderer.rotateView(glm::vec3(x,y,z));
}