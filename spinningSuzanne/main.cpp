#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "initHelper.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VBO_layout.h"
#include "VAO.h"
#include "EBO.h"
#include "model.h"

#include "obj.h"

int main(){
    // === Set up a GLFW window, and init GLAD ===
    char windowName[] = "3D spinning suzanne";
    GLFWwindow* window = glInitHelper::setup(windowName); // Setup function exists just to move all the boilerplate crap out of sight
    glEnable(GL_DEPTH_TEST); // Enable depth testing - emsures that objects are drawn in the right order
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe for model debugging

    obj suzanneObj("../cubeMod.obj");
    suzanneObj.readIndexData();
    suzanneObj.readPositionData();
    suzanneObj.readNormalData();
    suzanneObj.createVBO();

    model suzanne(
        suzanneObj.VBO, // vertices
        suzanneObj.EBO, // indices
        suzanneObj.layout // layout
    );
    suzanne.m_shader.createShader("GLSL/shader.vert.glsl", "GLSL/shader.frag.glsl");
    suzanne.m_shader.setUniform3f("lightPos", 1.0f, 10.0f, 2.0f);

    // === Define transforms ===
    // === perspective/view transforms:
    int m_viewport[4]; // Stores the viewport size
    glGetIntegerv(GL_VIEWPORT, m_viewport); // Gets the viewport size
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f); // Define the perspective projection matrix
    glm::mat4 view = glm::mat4(1.0f); // Define a view matrix for the scene
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f)); // note that we're translating the scene in the reverse direction of where we want to move
    // Only need to set this transform once (camera never moves in this example), so lets do it now outside of the loop:
    suzanne.setViewT(view);

    // Monkey spin
    glm::mat4 model = glm::mat4(1.0f); // Define a model matrix for the square
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // === Main loop === //
    while (!glfwWindowShouldClose(window)){
        // === Unbind/reset stuff (best practice or something) ===
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // === Render ===
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Set the background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

        // Set up the perspective projection matrix to ensure that the suzanne is rendered correctly if the window is resized:
        glGetIntegerv(GL_VIEWPORT, m_viewport); // Get the viewport size (maybe code to only do upon resize? - would required messy global variables)
        perspective = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f); // Update the perspective projection matrix
        suzanne.setProjectionT(perspective); // Set the perspective projection matrix

        // Rotate and draw the first suzanne :D
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.1f, 1.0f, 0.2f)); // Rotate the suzanne
        suzanne.setModelT(model); // Set the model matrix
        suzanne.draw();

        // === Swap buffers ===
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
