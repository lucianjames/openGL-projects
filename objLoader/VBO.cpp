#include "VBO.h"

void VBO::generate(std::vector<float> vertices, unsigned int size) {
    glGenBuffers(1, &this->ID); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->ID); // Bind the buffer to the GL_ARRAY_BUFFER target
    glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW); // Send the data to the buffer
}

VBO::~VBO() {
    glDeleteBuffers(1, &this->ID);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
}
