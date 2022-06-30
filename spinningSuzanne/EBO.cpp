#include "EBO.h"

EBO::EBO(std::vector<unsigned int> data, unsigned int size) : m_Size(size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
}

EBO::~EBO() {
    glDeleteBuffers(1, &ID);
}

void EBO::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
