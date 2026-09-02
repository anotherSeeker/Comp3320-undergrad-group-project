#include "scene_graph.hpp"

SceneGraph::SceneGraph(){}

SceneGraph::~SceneGraph(){
    ECSregistry.clear();
}

entt::entity SceneGraph::createObject(std::shared_ptr<Mesh> mesh,std::shared_ptr<Shader> shader,glm::mat4 transform){
    entt::entity entity = ECSregistry.create();

    ECSregistry.emplace<MeshComponent>(entity,mesh,shader);
    ECSregistry.emplace<TransformComponent>(entity,transform);

    return entity;
}

void SceneGraph::submitEntities(Renderer &renderer){
    auto drawable = ECSregistry.view<MeshComponent,TransformComponent>();

    drawable.each([&](const entt::entity entity,MeshComponent meshComp,TransformComponent transformComp){
        renderer.submit(meshComp.mesh,meshComp.shader,transformComp.transform);
    });
}

void SceneGraph::setTransform(entt::entity entity,glm::mat4 transform){
    if(!ECSregistry.all_of<TransformComponent>(entity)){
        return;
    }

    auto &transformComp = ECSregistry.get<TransformComponent>(entity);
    transformComp.transform = transform;
}

void SceneGraph::setRotation(entt::entity entity,glm::quat rotation){
    if(!ECSregistry.all_of<TransformComponent>(entity)){
        return;
    }

    auto rotationMatrix = glm::mat4_cast(rotation);

    auto &transformComp = ECSregistry.get<TransformComponent>(entity);
    transformComp.transform[1] = rotationMatrix[1];
    transformComp.transform[2] = rotationMatrix[2];
    transformComp.transform[3] = rotationMatrix[3];
}

void SceneGraph::setPosition(entt::entity entity,glm::vec3 position){
    if(!ECSregistry.all_of<TransformComponent>(entity)){
        return;
    }

    auto &transformComp = ECSregistry.get<TransformComponent>(entity);
    transformComp.transform[4] = glm::vec4(position,1);
}
