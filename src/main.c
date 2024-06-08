#include "glad.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "window.h"
#include "shader.h"
#include "renderer.h"
#include "input.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // glfw: initialize and create window
    GLFWwindow* window = initialize_window(SCR_WIDTH, SCR_HEIGHT, "Felix");
    initialize_glad();

    // build and compile our shader program
    unsigned int shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);

    // triangle-vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    // setup vertex buffers and configure vertex attributes
    unsigned int VBO, VAO;
    setupVertexArray(&VAO, &VBO, vertices, sizeof(vertices));

    // render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        render(shaderProgram, VAO);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return EXIT_SUCCESS;
}

