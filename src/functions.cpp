#include "functions.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#include "globals.hpp"

int juliaFunction(std::complex<double> &z, const std::complex<double> &c,
                  int maxIter) {
  int n = 0;
  while (std::abs(z) < 2 && n < maxIter) {
    z = z * z + c;
    n++;
  }
  return n;
}

void normalizePixels(std::vector<GLubyte> &pixels) {
  int max = *std::max_element(pixels.begin(), pixels.end());
  for (size_t i = 0; i < pixels.size(); ++i) {
    pixels[i] = (GLubyte)mapRange(pixels[i], 0, max, 0, 255);
  }
}

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
      aMap = mapRange(a0, -Globals::Constants::X_LIM, Globals::Constants::X_LIM,
                      0, Globals::WIDTH),
      bMap = mapRange(b0, Globals::Constants::Y_LIM, -Globals::Constants::Y_LIM,
                      0, Globals::HEIGHT);

      glfwSetCursorPos(window, aMap, bMap);
    }
  } break;
  }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  Globals::WIDTH = width;
  Globals::HEIGHT = height;
}

void renderJuliaSet(std::vector<GLubyte> &pixels, double a0, double b0) {

  for (int y = 0; y < Globals::HEIGHT; ++y) {
    for (int x = 0; x < Globals::WIDTH; ++x) {
      int idx = (y * Globals::WIDTH + x) * 3;

      double a = mapRange(x, 0, Globals::WIDTH, -Globals::Constants::X_LIM,
                          Globals::Constants::X_LIM);

      // for some reason, the y axis is inverted in the window
      // so we need to invert it in the mapping
      double b = mapRange(y, 0, Globals::HEIGHT, Globals::Constants::Y_LIM,
                          -Globals::Constants::Y_LIM);

      std::complex<double> c(a0, b0);
      std::complex<double> z(a, b);

      int n = juliaFunction(z, c, Globals::Constants::MAX_ITERATIONS);

      if (n == Globals::Constants::MAX_ITERATIONS) {
        pixels[idx] = 0;
        pixels[idx + 1] = 0;
        pixels[idx + 2] = 0;
      } else {
        int co = static_cast<int>(
            mapRange(n, 0, Globals::Constants::MAX_ITERATIONS, 0, 255));

        pixels[idx] = co;
        pixels[idx + 1] = co;
        pixels[idx + 2] = co;
      }
    }
  }
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
  title << "FPS: " << fps << "   Julia Set: " << a0 << (b0 > 0 ? " + " : " - ")
        << fabs(b0) << "i";

  glfwSetWindowTitle(window, title.str().c_str());
}
