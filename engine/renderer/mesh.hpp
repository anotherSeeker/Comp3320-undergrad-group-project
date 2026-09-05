#pragma once
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "shader.hpp"

struct Vertex{
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 UV{};

    bool operator==(const Vertex &other) const{
        return position == other.position 
            && normal == other.normal 
            && UV == other.UV;
    }
};

class Mesh{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    public:

    Mesh(std::vector<Vertex> vertices,std::vector<GLuint> indices);
    void draw(Shader &shader);

    void bind();
    void unbind();

    ~Mesh();
};