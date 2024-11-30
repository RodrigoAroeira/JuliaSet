#include "constants.hpp"
#include "functions.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <complex.h>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
  if (!glfwInit()) {
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(Constants::WIDTH, Constants::HEIGHT,
                                        "Julia Set", nullptr, nullptr);

  if (!window) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  float time = 0.0f;

  std::vector<GLubyte> pixels(Constants::WIDTH * Constants::HEIGHT * 3);
  GLubyte *pixelsData = pixels.data();
  int currentPixel = 0;
  double mouseX, mouseY;
  double a, b;
  double a0, b0;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

      glfwSetWindowShouldClose(window, true);
    }

    a0 = mapRange(mouseX, 0, Constants::WIDTH, -Constants::X_LIM,
                  Constants::X_LIM);
    b0 = mapRange(mouseY, 0, Constants::HEIGHT, Constants::Y_LIM,
                  -Constants::Y_LIM);

    std::ostringstream title;
    title << "Julia Set: " << a0 << (b0 > 0 ? " + " : " - ") << fabs(b0) << "i";
    glfwSetWindowTitle(window, title.str().c_str());

    for (int y = 0; y < Constants::HEIGHT; ++y) {
      for (int x = 0; x < Constants::WIDTH; ++x) {
        int idx = (y * Constants::WIDTH + x) * 3;

        a = mapRange(x, 0, Constants::WIDTH, -Constants::X_LIM,
                     Constants::X_LIM);

        // for some reason, the y axis is inverted in the window
        // so we need to invert it in the mapping
        b = mapRange(y, 0, Constants::HEIGHT, Constants::Y_LIM,
                     -Constants::Y_LIM);

        std::complex<double> c(a0, b0);
        std::complex<double> z(a, b);

        int n = juliaFunction(z, c, Constants::MAX_ITERATIONS);

        if (n == Constants::MAX_ITERATIONS) {
          pixels[idx] = 0;
          pixels[idx + 1] = 0;
          pixels[idx + 2] = 0;
        } else {
          int co = static_cast<int>(
              mapRange(n, 0, Constants::MAX_ITERATIONS, 0, 255));
          pixels[idx] = co;
          pixels[idx + 1] = co;
          pixels[idx + 2] = co;
        }
      }
    }

    normalizePixels(pixels);
    glRasterPos2i(-1, -1);
    glDrawPixels(Constants::WIDTH, Constants::HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 pixelsData);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
