#include "events.hpp"
#include "../../sunken_engine.hpp"
#include "../debugger.hpp"

std::unordered_map<const char*,Observer> EventManager::observers = {};
void (*EventManager::eventCallback)(int,int) = nullptr;

void EventManager::createObserver(const char* name,uint32_t ID){
    Observer observer = {
        .callbacks = std::list<uint32_t>(),
        .ID = ID,
    };

    EventManager::observers.try_emplace(name,observer);
}

bool EventManager::listen(const char* name,uint32_t callback){
    if(!EventManager::observers.contains(name)) return false;

    Observer &observer = EventManager::observers.at(name);

    observer.callbacks.push_back(callback);

    return true;
}

void EventManager::dispatch(const char* name){
    if(!EventManager::observers.contains(name)) return;

    Observer &observer = EventManager::observers.at(name);

    for(std::list<uint32_t>::iterator iterator = observer.callbacks.begin(); iterator != observer.callbacks.end();iterator++){
        EventManager::eventCallback(*iterator,observer.ID);
    }
}
