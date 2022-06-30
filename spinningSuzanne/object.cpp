#include "object.h"

// ===== Private functions =====

void object::setupVBO(){
    this->vbo = VBO(this->vertices.data(), this->vertices.size() * sizeof(float));
}

void object::setupEBO(){
    this->ebo = EBO(this->indices.data(), this->indices.size() * sizeof(unsigned int));
}

void object::setupVAO(){
    this->vao.addBuffer(this->vbo, this->layout);
}

// ===== Public functions =====

void object::update(){
    this->setupVBO();
    this->setupEBO();
    this->setupVAO();
}

void object::draw(){
    this->vao.bind();
    this->ebo.bind();
    this->shader.use();
    glDrawElements(GL_TRIANGLES, this->ebo.getSize(), GL_UNSIGNED_INT, 0);
}