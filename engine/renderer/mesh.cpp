#include "mesh.hpp"

Mesh::Mesh(std::vector<vertex> vertices,std::vector<GLuint> indices) : vertices(vertices), indices(indices){
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(vertex),vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),indices.data(),GL_STATIC_DRAW);

    glVertexAttribFormat(0,3,GL_FLOAT,GL_FALSE,0);
    glEnableVertexAttribArray(0);
}

void Mesh::draw(Shader shader){
    this->bind();
    shader.use();
    glDrawElements(GL_STATIC_DRAW,indices.size(),GL_FLOAT,indices.data());
}

void Mesh::bind(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
}

void Mesh::unbind(){
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    
}
