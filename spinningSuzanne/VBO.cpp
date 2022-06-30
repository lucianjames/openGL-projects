#include "VBO.h"

VBO::VBO(std::vector<float> vertices, unsigned int size) {
    glGenBuffers(1, &ID); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, ID); // Bind the buffer to the GL_ARRAY_BUFFER target
    glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW); // Send the data to the buffer
}

VBO::~VBO() {
    glDeleteBuffers(1, &ID);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
