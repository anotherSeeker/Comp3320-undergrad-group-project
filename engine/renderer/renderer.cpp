#include "renderer.hpp"

void Renderer::submit(std::shared_ptr<Mesh> mesh,std::shared_ptr<Shader> shader,glm::mat4 transform){
    RenderCommand command = {
        .sortOrder = shader->id,
        .type = commandType::DRAW,
        .data = drawCommand{
            .mesh = mesh,
            .shader = shader,
            .transform = transform,
        }
    };

    commandQueue.push_back(command);
}

void Renderer::begin(){
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::end(){
    while(!commandQueue.empty()){
        RenderCommand cmd = commandQueue.front();
        commandQueue.erase(commandQueue.begin());

        switch (cmd.type)
        {
        case commandType::DRAW:{
            drawCommand& commandData = std::get<drawCommand>(cmd.data);
            std::shared_ptr<Shader> shader = commandData.shader;

            shader->use();

            shader->SetMat4("model",commandData.transform);

            commandData.mesh->draw(*shader);

        }
            break;
        
        default:
            break;
        }



    }
}
