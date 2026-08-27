#pragma once

#include <queue>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <variant>
#include <memory>

#include "mesh.hpp"
#include "shader.hpp"

enum class commandType{
    DRAW = 0
};

struct drawCommand{
    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Shader> shader = nullptr;
    glm::mat4 transform;
};

struct RenderCommand{
    size_t sortOrder;
    commandType type;
    std::variant<drawCommand> data;
};

class Renderer{
    private:
        int width;
        int height;
        glm::mat4 viewportTransform;
        std::vector<RenderCommand> commandQueue;

        glm::mat4 computeViewportMatrix();

    public:
        void begin(int32_t width,int32_t height);
        void end();

        void submit(std::shared_ptr<Mesh> mesh,std::shared_ptr<Shader> shader,glm::mat4 transform);
};
