#include "./core/events/event_structs.hpp"
#include "./core/events/event_enums.hpp"

#include "./core/asset_handle.h"

extern "C" {
    __declspec(dllexport) bool skInit();
    __declspec(dllexport) void skRun();

    __declspec(dllexport) void skEventCallback(void (*eventCallback)(int,int,void*));
    __declspec(dllexport) void skListen(const char* eventName,int callback);

    __declspec(dllexport) void skLog(const char* message);

    __declspec(dllexport) void skMoveView(float x,float y,float z);
    __declspec(dllexport) void skRotateView(float x,float y,float z);

    __declspec(dllexport) SK_ASSET_HANDLE skLoadShader(const char* vertexFilePath,const char* fragmentFilePath);
    __declspec(dllexport) SK_ASSET_HANDLE skLoadMesh(const char* filePath);

    __declspec(dllexport) SK_ASSET_HANDLE skCreateObject(SK_ASSET_HANDLE mesh,SK_ASSET_HANDLE shader);
}