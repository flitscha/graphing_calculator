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
    float triangle_vertices[] = {
        -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f, // left  
         0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f, // right 
         0.0f,  0.5f, 0.0f,         0.0f, 0.0f, 1.0f, // top   
    }; 

    // setup vertex buffers and configure vertex attributes
    unsigned int VBO_triangles, VAO_triangles;
    setupVertexArray(&VAO_triangles, &VBO_triangles, triangle_vertices, sizeof(triangle_vertices));

    // line vertices
    float line_vertices[] = {
        // First line
        -0.5f, 0.0f, 0.0f,          1.0f, 0.5f, 0.0f,  
        0.5f, 0.5f, 0.0f,           0.5f, 1.0f, 0.0f,
        // Second line
        -0.5f, 0.0f, 0.0f,          0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f, 0.0f,         0.0f, 0.5f, 1.0f,

    };
    unsigned int VBO_lines, VAO_lines;
    setupVertexArray(&VAO_lines, &VBO_lines, line_vertices, sizeof(line_vertices));

    // render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        render(shaderProgram, VAO_triangles, VAO_lines, 1, 2);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO_triangles);
    glDeleteVertexArrays(1, &VAO_lines);
    glDeleteBuffers(1, &VBO_triangles);
    glDeleteBuffers(1, &VBO_lines);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return EXIT_SUCCESS;
}

