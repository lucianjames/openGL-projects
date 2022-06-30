#include "VBO.h"
#include "VBO_layout.h"
#include "VAO.h"
#include "EBO.h"

class model{
private:
    VBO m_vbo;
    VAO m_vao;
    EBO m_ebo;

public:
    VBO_layout m_layout; // LAYOUT MUST BE PUSHED!!! DONT FORGET!!!
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void generate(){
        m_vbo.generate(vertices, sizeof(float) * vertices.size());
        m_vao.addBuffer(m_vbo, m_layout);
        m_ebo.generate(indices, sizeof(unsigned int) * indices.size());
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void draw(){
        m_vao.bind();
        m_ebo.bind();
        glDrawElements(GL_TRIANGLES, m_ebo.getSize(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

};