#define GLM_ENABLE_EXPERIMENTAL

#include "renderer.hpp"
#include "../core/debugger.hpp"
#include "glm/gtx/string_cast.hpp"

glm::mat4 Renderer::computeViewportMatrix(){
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    view = glm::lookAt(viewportPosition,viewportPosition + viewportLookDirection,viewportUp);
    projection = glm::perspective(glm::radians(60.0f),(float)width/(float)height,0.1f,100.0f);

    return projection * view;
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
    glViewport(0,0,width,height);
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
    viewportPosition += translateBy;
}

void Renderer::moveView(glm::vec3 moveBy){
    glm::vec3 right = glm::normalize(glm::cross(viewportLookDirection,viewportUp));

    viewportPosition += right * moveBy.x;
    viewportPosition += viewportUp * moveBy.y;
    viewportPosition += -viewportLookDirection * moveBy.z;
}

void Renderer::rotateView(glm::vec3 rotateBy){
    glm::vec3 worldUp = glm::vec3(0,1,0);
    glm::mat4 rotationMatrix(1.0f);

    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f),rotateBy.x,viewportUp);
    viewportLookDirection = glm::normalize(glm::vec3(yawRotation * glm::vec4(viewportLookDirection,0.0f)));

    glm::vec3 lookXUp = glm::cross(viewportLookDirection,worldUp);
    glm::vec3 right = glm::length(lookXUp) < 1e-4f ? glm::vec3(1.0f,0.0f,0.0f) : glm::normalize(lookXUp);

    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f),rotateBy.y,right);
    viewportLookDirection = glm::normalize(glm::vec3(pitchRotation * glm::vec4(viewportLookDirection,0.0f)));
    
    viewportUp = glm::normalize(glm::cross(right,viewportLookDirection));
}

void Renderer::setViewPosition(glm::vec3 position){
    viewportPosition = position;
}

void Renderer::setViewOrientation(glm::vec3 orientation){
    glm::vec3 worldUp = glm::vec3(0,1,0);
    glm::mat4 rotationMatrix(1.0f);

    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f),orientation.x,viewportUp);
    viewportLookDirection = glm::normalize(glm::vec3(yawRotation * glm::vec4(0,0,1,0.0f)));

    glm::vec3 lookXUp = glm::cross(viewportLookDirection,worldUp);
    glm::vec3 right = glm::length(lookXUp) < 1e-4f ? glm::vec3(1.0f,0.0f,0.0f) : glm::normalize(lookXUp);

    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f),orientation.y,right);
    viewportLookDirection = glm::normalize(glm::vec3(pitchRotation * glm::vec4(viewportLookDirection,0.0f)));
    
    viewportUp = glm::normalize(glm::cross(right,viewportLookDirection));
}

void Renderer::viewLookAt(glm::vec3 position, glm::vec3 lookAt){
    viewportPosition = position;
    viewportLookDirection = glm::normalize(lookAt - position);
}