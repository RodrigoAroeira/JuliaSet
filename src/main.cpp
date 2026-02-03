// clang-format off
#include <glad/glad.h>
// clang-format on
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include "functions.hpp"
#include "globals.hpp"

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

    // --- Compile shaders from files ---
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, VERT_PATH);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, FRAG_PATH);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

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

    GLint uCLoc = glGetUniformLocation(shaderProgram, "uC");
    GLuint maxIterLoc = glGetUniformLocation(shaderProgram, "maxIter");
    GLuint zoomLoc = glGetUniformLocation(shaderProgram, "zoom");

    double mouseX_old = -1, mouseY_old = -1;
    double mouseX, mouseY;
    double a0, b0;

    int frames = 0;
    int CURRENT_FPS = 0;
    auto start = std::chrono::system_clock::now();
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetCursorPos(window, &mouseX, &mouseY);
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = now - start;
        if (elapsed.count() >= 1) {
            CURRENT_FPS = frames;
            frames = 0;
            start = now;
        } else {
            ++frames;
        }

        mapMouseToComplex(mouseX, mouseY, a0, b0);
        updateTitle(window, CURRENT_FPS, a0, b0);

        bool mouseStopped = (mouseX == mouseX_old && mouseY == mouseY_old);
        if (mouseStopped || Globals::PAUSED) {
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_WAIT));
            continue;
        }

        mouseX_old = mouseX;
        mouseY_old = mouseY;

        glUseProgram(shaderProgram);
        glUniform2f(uCLoc, static_cast<float>(a0), static_cast<float>(b0));
        glUniform1i(maxIterLoc, Globals::Constants::MAX_ITERATIONS);
        glUniform1f(zoomLoc, 1.5f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}

GLuint compileShader(GLenum type, const std::string &path) {
    std::ifstream file{path};
    if (!file.is_open()) {
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();

    auto s = ss.str();
    const char *csrc = s.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
        exit(1);
    }
    return shader;
}
