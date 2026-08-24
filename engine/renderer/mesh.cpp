#include "mesh.hpp"

Mesh::Mesh(std::vector<vertex> vertices,std::vector<GLuint> indices){
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

void Mesh::draw(){

}

Mesh::~Mesh(){
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
}
