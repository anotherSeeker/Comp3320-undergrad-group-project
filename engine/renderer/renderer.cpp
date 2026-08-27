#include "renderer.hpp"

glm::mat4 Renderer::computeViewportMatrix(){
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),(float)width/height,0.1f,100.0f);

    return projection * viewportTransform;
}

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

void Renderer::begin(int32_t width,int32_t height){
    this->width = width;
    this->height = height;
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::end(){
    glm::mat4 viewportMatrix = computeViewportMatrix();

    while(!commandQueue.empty()){
        RenderCommand cmd = commandQueue.front();
        commandQueue.erase(commandQueue.begin());

        switch (cmd.type)
        {
        case commandType::DRAW:{
            drawCommand& commandData = std::get<drawCommand>(cmd.data);
            std::shared_ptr<Shader> shader = commandData.shader;

            shader->use();

            shader->SetMat4("camera",commandData.transform);
            shader->SetMat4("model",commandData.transform);

            commandData.mesh->draw(*shader);

        }
            break;
        
        default:
            break;
        }



    }
}
