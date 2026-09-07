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

SK_ASSET skLoadShader(const char* vertexFilePath,const char* fragmentFilePath){
    return app.assetManager.loadShader(vertexFilePath,fragmentFilePath);
}

SK_ASSET skLoadMesh(const char* filepath){
    return app.assetManager.loadObj(filepath);
}

SK_ENTITY skCreateObject(SK_ASSET meshHandle,SK_ASSET shaderHandle){

    auto mesh = app.assetManager.getMesh(meshHandle);
    auto shader = app.assetManager.getShader(shaderHandle);

    entt::entity entity = app.scene.createObject(mesh,shader,glm::mat4(1.0f));

    return {static_cast<int>(entity)};
}

void skMoveObject(SK_ENTITY object,float x,float y,float z){
    app.scene.setPosition(static_cast<entt::entity>(object.ID),glm::vec3(x,y,z));
}

void skRotateObject(SK_ENTITY object,float x,float y,float z){
    app.scene.setRotation(static_cast<entt::entity>(object.ID),glm::quat(glm::vec3(x,y,z)));
}
