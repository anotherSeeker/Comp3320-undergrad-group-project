#pragma once

#include <queue>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/quaternion.hpp>

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
        glm::vec3 viewportPosition = glm::vec3(0,0,0);
        glm::vec3 viewportLookDirection = glm::vec3(0,0,1);
        glm::vec3 viewportUp = glm::vec3(0,1,0);

        std::vector<RenderCommand> commandQueue;

        glm::mat4 computeViewportMatrix();

    public:
        void begin(int32_t width,int32_t height);
        void end();

        void submit(std::shared_ptr<Mesh> mesh,std::shared_ptr<Shader> shader,glm::mat4 transform);

        void moveView(glm::vec3 moveBy);
        void translateView(glm::vec3 translateBy);
        void rotateView(glm::vec3 rotateBy);

        void setViewPosition(glm::vec3 position);
        void setViewOrientation(glm::vec3 orientation);

        void viewLookAt(glm::vec3 position, glm::vec3 lookAt);
};
