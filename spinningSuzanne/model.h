#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "shader.h"

class model{
protected:
    VBO m_vbo;
    VAO m_vao;
    VBO_layout m_layout;

    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;

public:
    shader m_shader;
    void readVBO(std::string path){
        // Create a vector to store the data from the file:
        std::vector<float> VBO_data;
        // Open the file:
        std::ifstream file(path);
        if(!file.is_open()){
            std::cout << "Error: Could not open file " << path << std::endl;
            return;
        }
        // Read the file:
        std::string line;
        while(std::getline(file, line)){
            std::stringstream ss(line);
            std::string token;
            while(std::getline(ss, token, ' ')){
                VBO_data.push_back(std::stof(token));
            }
        }
        // Close the file:
        file.close();
        // Generate the VBO:
        this->m_vbo.generate(VBO_data, VBO_data.size() * sizeof(float));
        // Create the VAO:
        this->m_layout; // THIS IS ASSUMED !!!!! HARD CODED !!!!! BAD !!!!!
        this->m_layout.pushFloat(3);
        this->m_layout.pushFloat(3);
        this->m_layout.pushFloat(2);
        this->m_vao.addBuffer(this->m_vbo, this->m_layout);
    }
    void setModelT(glm::mat4 model){
        this->m_model = model;
    }
    void setViewT(glm::mat4 view){
        this->m_view = view;
    }
    void setProjectionT(glm::mat4 projection){
        this->m_projection = projection;
    }
    void setTransformUniforms(){
        this->m_shader.use();
        this->m_shader.setUniformMat4fv("model", glm::value_ptr(this->m_model));
        this->m_shader.setUniformMat4fv("view", glm::value_ptr(this->m_view));
        this->m_shader.setUniformMat4fv("projection", glm::value_ptr(this->m_projection));
        glUseProgram(0);
    }
    void draw(){
        this->setTransformUniforms(); // <--- bad maybe ?
        this->m_shader.use();
        this->m_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, this->m_layout.getStride());
    }
};

class model_EBO : public model{
protected:
    EBO m_ebo;

public:
    void readEBO(std::string fileName){
        // Create a vector to store the data from the file:
        std::vector<unsigned int> EBO_data;
        // Open the file:
        std::ifstream file(fileName);
        if(!file.is_open()){
            std::cout << "Error: Could not open file " << fileName << std::endl;
            return;
        }
        // Read the file:
        std::string line;
        while(std::getline(file, line)){
            std::stringstream ss(line);
            std::string token;
            while(std::getline(ss, token, ' ')){
                EBO_data.push_back(std::stof(token));
            }
        }
        // Close the file:
        file.close();
        // Generate the EBO:
        this->m_ebo.generate(EBO_data, EBO_data.size() * sizeof(unsigned int));
    }
    void draw(){
        this->setTransformUniforms(); // <--- bad maybe ?
        this->m_shader.use();
        this->m_vao.bind();
        this->m_ebo.bind();
        glDrawElements(GL_TRIANGLES, this->m_ebo.getSize(), GL_UNSIGNED_INT, nullptr);
    }

};