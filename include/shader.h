#pragma once

#include "glad.h"
#include <stdbool.h>

extern const char *vertexShaderSource;
extern const char *fragmentShaderSource;

unsigned int create_shader_program(const char* vertexSource, const char* fragmentSource);
bool check_compile_errors(unsigned int shader, const char* type);