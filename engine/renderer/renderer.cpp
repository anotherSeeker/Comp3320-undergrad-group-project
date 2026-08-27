#define GLM_ENABLE_EXPERIMENTAL

#include "renderer.hpp"
#include "../core/debugger.hpp"
#include "glm/gtx/string_cast.hpp"

glm::mat4 Renderer::computeViewportMatrix(){
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),(float)width/(float)height,0.1f,100.0f);

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

            shader->SetMat4("camera",viewportMatrix);
            shader->SetMat4("model",commandData.transform);

            commandData.mesh->draw(*shader);

        }
            break;
        
        default:
            break;
        }



    }
}

void Renderer::translateView(glm::vec3 translateBy){
    viewportTransform = glm::translate(viewportTransform,translateBy);
}

void Renderer::rotateView(glm::quat rotateBy){
    glm::mat4 rotationMatrix = glm::mat4_cast(rotateBy);
    viewportTransform = viewportTransform * rotationMatrix;
}

void Renderer::setViewPosition(glm::vec3 position){
    viewportTransform[3] = glm::vec4(position,1.0f);
}

void Renderer::setViewOrientation(glm::quat orientation){
    glm::quat normOrientation = glm::normalize(orientation);

    glm::vec3 translation = viewportTransform[3];

    glm::mat4 rotationMatrix = glm::mat4_cast(normOrientation);

    viewportTransform = glm::translate(rotationMatrix,translation);
}

void Renderer::viewLookAt(glm::vec3 position, glm::vec3 lookAt){
    viewportTransform = glm::lookAt(position,lookAt,glm::vec3(0,1,0));
}