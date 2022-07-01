#include "model.h"

model::model(std::vector<float> vertices, std::vector<unsigned int> indices, VBO_layout layout){
        this->m_vbo.generate(vertices, sizeof(float) * vertices.size());
        this->m_vao.addBuffer(this->m_vbo, layout);
        this->m_ebo.generate(indices, sizeof(unsigned int) * indices.size());
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

void model::setModelT(glm::mat4 model){
    this->m_model = model;
}

void model::setViewT(glm::mat4 view){
    this->m_view = view;
}

void model::setProjectionT(glm::mat4 projection){
    this->m_projection = projection;
}

void model::setTransformUniforms(){
    this->m_shader.use();
    this->m_shader.setUniformMat4fv("model", glm::value_ptr(this->m_model));
    this->m_shader.setUniformMat4fv("view", glm::value_ptr(this->m_view));
    this->m_shader.setUniformMat4fv("projection", glm::value_ptr(this->m_projection));
    glUseProgram(0);
}

void model::draw(){
        this->setTransformUniforms();
        this->m_shader.use();
        this->m_vao.bind();
        this->m_ebo.bind();
        glDrawElements(GL_TRIANGLES, this->m_ebo.getSize(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }
