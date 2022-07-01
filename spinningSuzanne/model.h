#include "VBO.h"
#include "VBO_layout.h"
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"

class model{
private:
    VBO m_vbo;
    VAO m_vao;
    EBO m_ebo;

public:
    model(std::vector<float> vertices, std::vector<unsigned int> indices, VBO_layout layout);
    void draw();
};