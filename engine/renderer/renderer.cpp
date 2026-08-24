#include "renderer.hpp"

void Renderer::drawMesh(Mesh mesh,Shader shader){
    mesh.draw(shader);
}