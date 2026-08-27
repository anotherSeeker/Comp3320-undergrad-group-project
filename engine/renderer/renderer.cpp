#include "renderer.hpp"

void Renderer::submit(Mesh &mesh,Shader &shader){
    mesh.draw(shader);
}

void Renderer::begin(){
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::end(){
    while(!commandQueue.empty()){
        commandQueue.empty();
    }
}
