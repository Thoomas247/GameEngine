#include <iostream>

#include "glad/glad.h"
#include "glfw/glfw3.h"

// Global constants:
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Functions:
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
