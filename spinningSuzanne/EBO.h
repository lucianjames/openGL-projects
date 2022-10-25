#pragma once
#include <glad/glad.h>
#include <vector>

class EBO{
private:
    unsigned int ID;
    unsigned int m_size;
public:
    void generate(std::vector<unsigned int> data, unsigned int size){
        this->m_size = size;
        glGenBuffers(1, &this->ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
    }
    void bind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
    }
    inline unsigned int getSize() const { return this->m_size; }
    ~EBO(){
        glDeleteBuffers(1, &this->ID);
    }
};