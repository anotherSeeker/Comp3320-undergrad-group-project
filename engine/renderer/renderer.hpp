#pragma once

/*
this file may seem useless but this is a placeholder until i get an actual command queue going
*/

#include "mesh.hpp"
#include "shader.hpp"

class Renderer{
    private:
    public:
        void drawMesh(Mesh mesh,Shader shader);
};
