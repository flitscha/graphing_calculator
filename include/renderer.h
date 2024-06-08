#pragma once

#include "glad.h"
#include <stdlib.h> // for size_t

void setupVertexArray(unsigned int* VAO, unsigned int* VBO, float* vertices, size_t size);
void render(unsigned int shaderProgram, unsigned int VAO);
