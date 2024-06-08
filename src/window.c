#include "glad.h" // glad must be included before <GLFW/glfw3.h>
#include "window.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


GLFWwindow* initialize_window(int width, int height, const char* title) {
    if (!glfwInit()) {
        EXIT_ERROR("glfwInit\n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        EXIT_ERROR("Failed to create GLFW window\n");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void initialize_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        EXIT_ERROR("Failed to initialize GLAD\n");
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}