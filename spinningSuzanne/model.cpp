#include "model.h"

model::model(std::vector<float> vertices, std::vector<unsigned int> indices, VBO_layout layout){
        this->m_vbo.generate(vertices, sizeof(float) * vertices.size());
        this->m_vao.addBuffer(this->m_vbo, layout);
        this->m_ebo.generate(indices, sizeof(unsigned int) * indices.size());
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

void model::draw(){
        this->m_vao.bind();
        this->m_ebo.bind();
        glDrawElements(GL_TRIANGLES, this->m_ebo.getSize(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }