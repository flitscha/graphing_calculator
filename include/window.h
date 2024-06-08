#pragma once

#include <GLFW/glfw3.h>

GLFWwindow* initialize_window(int width, int height, const char* title);
void initialize_glad();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);