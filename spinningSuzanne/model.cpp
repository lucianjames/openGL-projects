#include "model.h"
#include <fstream>
#include <charconv>

void model::readVBO(std::string fileName){
    // Create a vector to store the data from the file
    std::vector<float> VBOData;
    // Open the file
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cout << "Error: Could open file: " << fileName << std::endl;
        return;
    }
    // Read the file
    std::string line;
    while(std::getline(file, line)){
        // Split the line into tokens
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ' ')){
            // Add the token to the vector
            VBOData.push_back(std::stof(token));
        }
    }
    // Close the file
    file.close();
    // Create the VBO
    m_vbo.generate(VBOData, sizeof(float) * VBOData.size());
    // Create the VAO
    VBO_layout layout; // THIS IS ASSUMED !!!!! HARD CODED !!!!!
    layout.pushFloat(3); 
    layout.pushFloat(3);
    layout.pushFloat(2);
    this->m_vao.addBuffer(this->m_vbo, layout);
}

void model::readEBO(std::string fileName){
    // Create a vector to store the data from the file
    std::vector<unsigned int> EBOData;
    // Open the file
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cout << "Error: Could open file: " << fileName << std::endl;
        return;
    }
    // Read the file
    std::string line;
    while(std::getline(file, line)){
        // Split the line into tokens
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ' ')){
            // Add the token to the vector
            EBOData.push_back(std::stoi(token));
        }
    }
    // Close the file
    file.close();
    // Create the EBO
    m_ebo.generate(EBOData, sizeof(unsigned int) * EBOData.size());
    // Debug: Print the EBO data
    std::cout << "EBO Data: " << std::endl;
    for(int i = 0; i < EBOData.size(); i++){
        std::cout << EBOData[i] << " ";
    }
    std::cout << std::endl;
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
