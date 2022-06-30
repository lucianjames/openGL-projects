#include "VAO.h"
#include "EBO.h"
#include "VBO.h"
#include "VBO_layout.h"
#include "shaderClass.h"

// The object class stores all the information required to render some "object" - VAO, VBO, EBO, shader, etc.

class object{
private:
    VBO vbo;
    EBO ebo;
    VAO vao;
    void setupVBO();
    void setupEBO();
    void setupVAO();
    
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    VBO_layout layout;
    shaderClass shader;

    void update();
    void draw();
};
