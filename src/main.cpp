#include "constants.hpp"
#include "functions.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
#include <complex.h>
#include <vector>

int main() {

  GLFWwindow *window = nullptr;

  setupWindow(window);

  std::vector<GLubyte> pixels(Constants::WIDTH * Constants::HEIGHT * 3);
  GLubyte *pixelsData = pixels.data();
  double mouseX, mouseY;
  double a0, b0;

  int cnter = 0;
  int CURRENT_FPS = 0;
  auto start = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    auto current = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = current - start;

    if (elapsed.count() >= 1) {
      CURRENT_FPS = cnter;
      cnter = 0;
      start = std::chrono::high_resolution_clock::now();
    } else {
      cnter++;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glfwGetCursorPos(window, &mouseX, &mouseY);

    mapMouseToComplex(mouseX, mouseY, a0, b0);

    updateTitle(window, CURRENT_FPS, a0, b0);

    renderJuliaSet(pixels, a0, b0);

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
