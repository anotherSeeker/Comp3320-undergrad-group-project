#pragma once
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>

struct vertex{
    glm::vec3 position;
};

class Mesh{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    public:

    Mesh(std::vector<vertex> vertices,std::vector<GLuint> indices);
    void draw();
    ~Mesh();
};