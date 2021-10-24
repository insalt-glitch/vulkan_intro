#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

#include <GLFW/glfw3.h>

void main_loop(GLFWwindow* wnd) {
    while (!glfwWindowShouldClose(wnd)) {
        glfwPollEvents();
    }
}

#endif // MAIN_LOOP_H_