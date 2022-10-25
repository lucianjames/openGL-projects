#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>

// Vertex buffer object class, used to store vertex data:
class VBO{
private:
    unsigned int ID;
public:
    ~VBO(){
        glDeleteBuffers(1, &ID);
    }
    void generate(std::vector<float> vertices, unsigned int size){
        glGenBuffers(1, &this->ID); // Generate 1 buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->ID); // Bind the buffer to the GL_ARRAY_BUFFER target
        glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW); // Send the data to the buffer
    }
    void bind(){
        glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    }
};

struct VBO_element{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    static unsigned int getSizeOfType(unsigned int type){
        switch(type){
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};

// VBO layout class, used to store the layout of the vertex data so that the shader knows how to interpret it:
class VBO_layout{
private:
    std::vector<VBO_element> elements;
    unsigned int stride;
public:
    VBO_layout() : stride(0) {};
    void pushFloat(unsigned int count){
        this->elements.push_back({GL_FLOAT, count, GL_FALSE});
        this->stride += VBO_element::getSizeOfType(GL_FLOAT) * count;
    }
    void pushInt(unsigned int count){
        this->elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        this->stride += VBO_element::getSizeOfType(GL_UNSIGNED_INT) * count;
    }
    void pushByte(unsigned int count){
        this->elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        this->stride += VBO_element::getSizeOfType(GL_UNSIGNED_BYTE) * count;
    }
    inline const std::vector<VBO_element> getElements() const { return this->elements; }
    inline unsigned int getStride() const { return this->stride; }
};
