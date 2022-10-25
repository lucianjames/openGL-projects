#pragma once
#include <glad/glad.h>
#include "VBO.h"

class VAO{
private:
    unsigned int ID;
public:
    VAO(){
        glGenVertexArrays(1, &this->ID);
    }
    ~VAO(){
        glDeleteVertexArrays(1, &this->ID);
    }
    void bind(){
        glBindVertexArray(this->ID);
    }
    void addBuffer(VBO& vbo, const VBO_layout& layout){
        this->bind();
        vbo.bind();
        const auto& elements = layout.getElements();
        unsigned long long int offset = 0;
        for(unsigned int i = 0; i < elements.size(); i++){
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
            offset += VBO_element::getSizeOfType(element.type) * element.count;
        }
    }
};