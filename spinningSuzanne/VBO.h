#pragma once
#include <glad/glad.h>
#include <vector>

class VBO {
private:
    unsigned int ID;
public:
    VBO(std::vector<float> vertices, unsigned int size);
    ~VBO();
    void bind();
};