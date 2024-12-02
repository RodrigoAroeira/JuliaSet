#pragma once

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <complex>
#include <vector>

int juliaFunction(std::complex<double> &z, const std::complex<double> &c,
                  int maxIter);

void normalizePixels(std::vector<GLubyte> &pixels);

double mapRange(double value, double min1, double max1, double min2,
                double max2);

void mapMouseToComplex(double mouseX, double mouseY, double &a0, double &b0);

void keypressCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods);

void renderJuliaSet(std::vector<GLubyte> &pixels, double a0, double b0);

void setupWindow(GLFWwindow *&window);

void updateTitle(GLFWwindow *&window, int fps, double a0, double b0);
