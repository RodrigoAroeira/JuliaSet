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

    case GLFW_KEY_C: {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mapMouseToComplex(mouseX, mouseY, mouseX, mouseY);
        std::string clipboard =
            std::to_string(mouseX) + " " + std::to_string(mouseY);

        glfwSetClipboardString(window, clipboard.c_str());
    } break;

    case GLFW_KEY_V: {
        const char *clipboard = glfwGetClipboardString(window);
        if (clipboard) {
            std::istringstream iss(clipboard);
            double a0, b0;
            iss >> a0 >> b0;
            double aMap, bMap;
            aMap = mapRange(a0, -Globals::Constants::X_LIM,
                            Globals::Constants::X_LIM, 0, Globals::WIDTH),
            bMap = mapRange(b0, Globals::Constants::Y_LIM,
                            -Globals::Constants::Y_LIM, 0, Globals::HEIGHT);

            glfwSetCursorPos(window, aMap, bMap);
        }
    } break;
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    Globals::WIDTH = width;
    Globals::HEIGHT = height;
    Globals::resized = true;
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
