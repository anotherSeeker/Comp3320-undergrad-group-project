#include "./core/events/event_structs.hpp"

extern "C" {
    bool skInit();
    void skRun();

    void skEventCallback(void (*eventCallback)(int,int,void*));
    void skListen(const char* eventName,int callback);

    void skLog(const char* message);

    void skMoveView(float x,float y,float z);
    void skRotateView(float x,float y,float z);
}