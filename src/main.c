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
#include "object_manager.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // glfw: initialize and create window
    GLFWwindow* window = initialize_window(SCR_WIDTH, SCR_HEIGHT, "Felix");
    initialize_glad();

    // build and compile our shader program
    unsigned int shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);

    // setup vertex buffers and configure vertex attributes
    //setupVertexArray(&VAO_triangles, &VBO_triangles, triangle_vertices, sizeof(triangle_vertices));

    // line vertices
    /*
    float line_vertices[] = {
        // First line
        -0.5f, 0.0f, 0.0f,          1.0f, 0.5f, 0.0f,  
        0.5f, 0.5f, 0.0f,           0.5f, 1.0f, 0.0f,
        // Second line
        -0.5f, 0.0f, 0.0f,          0.0f, 1.0f, 0.5f,
        -0.5f, -0.5f, 0.0f,         0.0f, 0.5f, 1.0f,

    };*/


    // test the object manager
    object_manager_t object_manager;
    init_object_manager(&object_manager);


    triangle_t t1 = {
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}, 0
    };
    triangle_t t2 = {
        {-1.0f, -1.0f, 0.0f,} , {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}, 0
    };
    add_triangle(&object_manager, &t1);
    add_triangle(&object_manager, &t2);

    remove_triangle(&object_manager, &t1);
    // next goal: add random triangle with space, delete a random triangle with 'd'
    // and fix the segmentation fault in cut_line_storage()



    unsigned int VBO_triangles, VAO_triangles;
    unsigned int VBO_lines, VAO_lines;
    size_t num_triangles = object_manager.num_deleted_triangles + object_manager.num_visible_triangles;
    size_t num_lines = object_manager.num_deleted_lines + object_manager.num_visible_lines;
    setupVertexArray(&VAO_triangles, &VBO_triangles, object_manager.triangle_vertices, sizeof(float) * num_triangles * 18);
    setupVertexArray(&VAO_lines, &VBO_lines, object_manager.line_vertices, sizeof(float) * num_lines * 12);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        render(shaderProgram, VAO_triangles, VAO_lines, num_triangles, num_lines);
 
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

