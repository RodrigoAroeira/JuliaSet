#pragma once

#include <GL/gl.h>
#include <GLFW/glfw3.h>

double mapRange(double value, double min1, double max1, double min2,
                double max2);

void mapMouseToComplex(double mouseX, double mouseY, double &a0, double &b0);

void setupWindow(GLFWwindow *&window);

void updateTitle(GLFWwindow *&window, int fps, double a0, double b0);
