#pragma once
#include <entt/entt.hpp>

#include "../renderer/renderer.hpp"

#include <memory>

struct MeshComponent {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
};

struct TransformComponent {
    glm::mat4 transform;
};

class SceneGraph{
    private:
    entt::registry ECSregistry;

    public:
    SceneGraph();
    ~SceneGraph();

    entt::entity createObject(std::shared_ptr<Mesh> mesh,std::shared_ptr<Shader> shader,glm::mat4 transform);
    void submitEntities(Renderer &renderer);
};