#include "renderer.h"
#include <GLFW/glfw3.h>


void setupVertexArray(unsigned int* VAO, unsigned int* VBO, float* vertices, size_t size) {
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // attrib: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // attrib: color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void render(unsigned int shaderProgram, unsigned int VAO_triangles, 
            unsigned int VAO_lines, int num_triangles, int num_lines) {
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // triangles
    glBindVertexArray(VAO_triangles);
    glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);

    // lines
    glBindVertexArray(VAO_lines);
    glDrawArrays(GL_LINES, 0, num_lines * 2);
}