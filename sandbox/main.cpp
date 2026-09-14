#include <sunken_engine.hpp>
#include <iostream>
#include <string>
#include <format>

#include <unordered_map>
#include <windows.h>

#include <charconv>

extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

bool mouseheld = false;
double cameraSpeed = 10;

std::unordered_map<char,bool> keyMap = {
    {'W', false},
    {'A', false},
    {'S', false},
    {'D', false},
    {'Q', false},
    {'E', false},
};

void EventCallback(int callback,int eventID,void* data){
    if (eventID == SK_EVENT_KEY_PRESS){
        char key = *static_cast<int*>(data);

        if(keyMap.contains(key)){
            keyMap.at(key) = true;
        }
        
    } else if (eventID == SK_EVENT_KEY_LIFTED){
        char key = *static_cast<int*>(data);

        if(keyMap.contains(key)){
            keyMap.at(key) = false;
        }

    } else if (eventID == SK_EVENT_MOUSE_PRESS){
        mouseheld = true;
        int button = *static_cast<int*>(data);

    } else if (eventID == SK_EVENT_MOUSE_LIFTED){
        mouseheld = false;
        int button = *static_cast<int*>(data);

    } else if (eventID == SK_EVENT_MOUSE_MOVE){
        if(!mouseheld) return;
        SK_MOUSE_MOVE_EVENT mouseData = *static_cast<SK_MOUSE_MOVE_EVENT*>(data);
         
        skRotateView(-mouseData.deltaY * 0.005,-mouseData.deltaX * 0.005,0);
    } else if (eventID == SK_EVENT_PRERENDER){
        double deltaTime = *static_cast<double*>(data);
 
        if(keyMap.at('W') == true){
            skMoveView(0,0,-cameraSpeed * deltaTime);
        }

        if (keyMap.at('S') == true){
            skMoveView(0,0,cameraSpeed * deltaTime);
        }

        if (keyMap.at('A') == true){
            skMoveView(-cameraSpeed * deltaTime,0,0);
        }

        if (keyMap.at('D') == true){
            skMoveView(cameraSpeed * deltaTime,0,0);
        }

        if (keyMap.at('Q') == true){
            skMoveView(0,-cameraSpeed * deltaTime,0);
        }
        if (keyMap.at('E') == true){
            skMoveView(0,cameraSpeed * deltaTime,0);
        }

    }

}

int main(){
    if (!skInit()) return -1;

    skListen("KeyPress",1);
    skListen("KeyLifted",2);
    skListen("MousePress",3);
    skListen("MouseLifted",4);
    skListen("MouseMove",5);
    skListen("PreRender",6);

    skEventCallback(EventCallback);

    SK_ASSET shader = skLoadShader("../assets/shaders/default.vert","../assets/shaders/default.frag");
    SK_ASSET sphere = skLoadMesh("../assets/meshes/uvsphere.obj");
    SK_ASSET suzanne = skLoadMesh("../assets/meshes/suzanne.obj");

    SK_ENTITY A = skCreateObject(sphere,shader);
    SK_ENTITY B = skCreateObject(suzanne,shader);

    skMoveObject(B,5.0f,0.0f,0.0f);

    skRun();
}

