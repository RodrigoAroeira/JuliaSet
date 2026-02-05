#include "functions.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <sstream>

#include "globals.hpp"

double mapRange(double value, double in_min, double in_max, double out_min,
                double out_max) {

    if (in_min == in_max) {
        return out_min;
    }

    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void mapMouseToComplex(double mouseX, double mouseY, double &a0, double &b0) {
    a0 = mapRange(mouseX, 0, Globals::WIDTH, -Globals::Constants::X_LIM,
                  Globals::Constants::X_LIM);
    b0 = mapRange(mouseY, 0, Globals::HEIGHT, Globals::Constants::Y_LIM,
                  -Globals::Constants::Y_LIM);
}

void keypressCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods) {

    // No plans for other events, so just return
    if (action != GLFW_PRESS) {
        return;
    }

    switch (key) {
    case GLFW_KEY_F: {
        if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
            glfwRestoreWindow(window);
        } else {
            glfwMaximizeWindow(window);
        }
    } break;
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose(window, true);
        break;

    case GLFW_KEY_SPACE:
        Globals::PAUSED = !Globals::PAUSED;
        break;
    case GLFW_KEY_R:
        Globals::RELOAD = true;
        break;
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    Globals::WIDTH = width;
    Globals::HEIGHT = height;
}

void setupWindow(GLFWwindow *&window) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(Globals::WIDTH, Globals::HEIGHT, "Julia Set",
                              nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keypressCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void updateTitle(GLFWwindow *&window, int fps, double a0, double b0) {
    std::ostringstream title;
    title << "FPS: " << fps << "   Julia Set: " << a0
          << (b0 > 0 ? " + " : " - ") << fabs(b0) << "i";

    glfwSetWindowTitle(window, title.str().c_str());
}
