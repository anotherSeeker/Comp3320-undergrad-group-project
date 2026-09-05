#include "mesh.hpp"

Mesh::Mesh(std::vector<vertex> vertices,std::vector<GLuint> indices) : vertices(vertices), indices(indices){
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(vertex),vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),indices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)offsetof(vertex,normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)offsetof(vertex,UV));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void Mesh::draw(Shader &shader){
    this->bind();
    shader.use();
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
}

void Mesh::bind(){
    glBindVertexArray(VAO);
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
