#pragma once
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "shader.hpp"

struct vertex{
    glm::vec3 position;
};

class Mesh{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<vertex> vertices;
    std::vector<GLuint> indices;

    public:

    Mesh(std::vector<vertex> vertices,std::vector<GLuint> indices);
    void draw(Shader &shader);

    void bind();
    void unbind();

    ~Mesh();
};