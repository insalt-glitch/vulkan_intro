#ifndef CLEANUP_H_
#define CLEANUP_H_

#include <GLFW/glfw3.h>

void cleanup(GLFWwindow* wnd) {
    glfwDestroyWindow(wnd);
    glfwTerminate();
}

#endif // CLEANUP_H_