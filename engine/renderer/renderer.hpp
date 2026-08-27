#pragma once

#include <queue>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.hpp"
#include "shader.hpp"

struct RenderCommand{

};

class Renderer{
    private:
        std::queue<RenderCommand> commandQueue;
    public:
        void begin();
        void end();

        void submit(Mesh &mesh,Shader &shader);
};
