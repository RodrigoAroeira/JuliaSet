// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

#include "functions.hpp"
#include "globals.hpp"
#include "shaders.hpp"

constexpr int FPS = 60;
constexpr int FRAME_WAIT = 1000 / 60;

#ifndef FRAG_PATH
#define FRAG_PATH "shaders/fragment.glsl"
#endif

#ifndef VERT_PATH
#define VERT_PATH "shaders/vertex.glsl"
#endif

int main() {
    GLFWwindow *window = nullptr;
    setupWindow(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    ShaderProgram shader(VERT_PATH, FRAG_PATH);

    float quadVertices[] = {
        -1.0f, -1.0f, // bottom-left
        1.0f,  -1.0f, // bottom-right
        -1.0f, 1.0f,  // top-left
        1.0f,  1.0f   // top-right
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    auto getUniforms = [&]() {
        GLint uCLoc = shader.getLoc("uC");
        GLint maxIterLoc = shader.getLoc("maxIter");
        GLint zoomLoc = shader.getLoc("zoom");
        GLint panLoc = shader.getLoc("pan");
        return std::tuple{uCLoc, maxIterLoc, zoomLoc, panLoc};
    };

    auto [uCLoc, maxIterLoc, zoomLoc, panLoc] = getUniforms();

    double mouseX, mouseY;
    double a0, b0;

    int frames = 0;
    int CURRENT_FPS = 0;
    auto start = std::chrono::system_clock::now();
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (Globals::RELOAD) {
            if (!shader.reload())
                std::cout << "Reload failed\n";
            Globals::RELOAD = false;
            std::tie(uCLoc, maxIterLoc, zoomLoc, panLoc) = getUniforms();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = now - start;
        if (elapsed.count() >= 1) {
            CURRENT_FPS = frames;
            frames = 0;
            start = now;
        } else {
            ++frames;
        }

        glfwGetCursorPos(window, &mouseX, &mouseY);
        updateTitle(window, CURRENT_FPS, a0, b0);

        if (!Globals::PAUSED) {
            mapMouseToComplex(mouseX, mouseY, a0, b0);
        }

        shader.use();
        glUniform2f(uCLoc, static_cast<float>(a0), static_cast<float>(b0));
        glUniform1i(maxIterLoc, Globals::Constants::MAX_ITERATIONS);
        glUniform2f(panLoc, Globals::PAN_X, Globals::PAN_Y);
        glUniform1f(zoomLoc, Globals::ZOOM);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
}
