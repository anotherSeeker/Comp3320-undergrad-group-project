#include <sunken_engine.hpp>
#include <iostream>

#include "./core/debugger.hpp"

#include "./core/app.hpp"
#include "./core/events/events.hpp"

void skInit(){
    App app;

    if(!app.init(800,600,"App")){
        std::cerr << "failed to open app\n";
        return;
    }

    app.run();
}

void skEventCallback(void (*eventCallback)(int,int)){
    EventManager::eventCallback = eventCallback;
}

void skLog(const char* message){
    Debugger::print(message);
}