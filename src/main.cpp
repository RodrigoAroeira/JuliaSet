#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <complex.h>

#include <chrono>
#include <cmath>
#include <vector>

#include "functions.hpp"
#include "globals.hpp"

int main() {

  GLFWwindow *window = nullptr;

  setupWindow(window);

  std::vector<GLubyte> pixels;
  double mouseX_old = 0, mouseY_old = 0;
  double mouseX, mouseY;
  double a0, b0;

  int cnter = 0;
  int CURRENT_FPS = 0;
  auto start = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    pixels.resize(Globals::WIDTH * Globals::HEIGHT * 3);
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

    if (mouseX == mouseX_old && mouseY == mouseY_old)
      continue;

    mouseX_old = mouseX;
    mouseY_old = mouseY;

    mapMouseToComplex(mouseX, mouseY, a0, b0);

    updateTitle(window, CURRENT_FPS, a0, b0);

    renderJuliaSet(pixels, a0, b0);

    normalizePixels(pixels);

    glRasterPos2i(-1, -1);

    glDrawPixels(Globals::WIDTH, Globals::HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 pixels.data());

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
