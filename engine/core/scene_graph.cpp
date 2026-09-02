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