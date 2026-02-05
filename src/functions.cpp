#include "functions.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <tuple>

#include "globals.hpp"

std::tuple<double, double> getLimits() {
    double x = Globals::Constants::X_LIM * Globals::ZOOM;
    double y = Globals::Constants::Y_LIM * Globals::ZOOM;
    return {x, y};
}

double mapRange(double value, double in_min, double in_max, double out_min,
                double out_max) {

    if (in_min == in_max) {
        return out_min;
    }

    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void mapMouseToComplex(double mouseX, double mouseY, double &a0, double &b0) {
    auto [xlim, ylim] = getLimits();

    a0 = mapRange(mouseX, 0, Globals::WIDTH, -xlim, xlim);
    b0 = mapRange(mouseY, 0, Globals::HEIGHT, ylim, -ylim);
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

void scrollCallback(GLFWwindow *, double, double yOffset) {
    constexpr double zoomFactor = 1.1;

    if (yOffset > 0)
        Globals::ZOOM /= zoomFactor;
    else
        Globals::ZOOM *= zoomFactor;

    std::cout << "Zoom: " << Globals::ZOOM << '\n';
}

static bool DRAGGING = false;
static double LAST_X, LAST_Y;

void mouseButCallback(GLFWwindow *window, int button, int action, int) {
    switch (action) {
    case GLFW_PRESS: {
        switch (button) {
        case GLFW_MOUSE_BUTTON_1: {
            if (!DRAGGING) {
                glfwGetCursorPos(window, &LAST_X, &LAST_Y);
            }
            DRAGGING = true;
        } break;
        case GLFW_MOUSE_BUTTON_3:
            Globals::ZOOM = Globals::Constants::ZOOM_DEFAULT;
            Globals::PAN_X = 0;
            Globals::PAN_Y = 0;
            break;
        }
    } break;
    case GLFW_RELEASE:
        if (button == GLFW_MOUSE_BUTTON_1)
            DRAGGING = false;
        break;
    }
}

void cursorPosCallback(GLFWwindow *window, double x, double y) {
    if (!DRAGGING)
        return;

    double dx = x - LAST_X;
    double dy = y - LAST_Y;

    LAST_X = x;
    LAST_Y = y;

    float ndc_dx = (float)dx / Globals::WIDTH;
    float ndc_dy = (float)dy / Globals::HEIGHT;

    Globals::PAN_X -= ndc_dx * Globals::ZOOM;
    Globals::PAN_Y += ndc_dy * Globals::ZOOM;
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
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
}

void updateTitle(GLFWwindow *&window, int fps, double a0, double b0) {
    std::ostringstream title;
    title << "FPS: " << fps << "   Julia Set: " << a0
          << (b0 > 0 ? " + " : " - ") << fabs(b0) << "i";

    glfwSetWindowTitle(window, title.str().c_str());
}
