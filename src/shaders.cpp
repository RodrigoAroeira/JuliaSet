#include "shaders.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

static std::string loadFile(const std::string &path) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

ShaderProgram::ShaderProgram(std::string vertPath, std::string fragPath)
    : mVertPath(vertPath), mFragPath(fragPath) {
    reload();
}

ShaderProgram::~ShaderProgram() {
    if (mProgram) {
        glDeleteProgram(mProgram);
    }
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept {
    if (this != &other) {
        glDeleteProgram(mProgram);
        mProgram = other.mProgram;
        other.mProgram = 0;
        mVertPath = std::move(other.mVertPath);
        mFragPath = std::move(other.mFragPath);
    }
    return *this;
}

GLuint ShaderProgram::compile(GLenum type, const std::string &path) {
    std::string src = loadFile(path);
    const char *csrc = src.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);
    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Shader compile failed (" << path << "):\n" << log << "\n";
        return 0;
    }
    return shader;
}

GLuint ShaderProgram::link(GLuint vert, GLuint frag) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint ok;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Program link failed:\n" << log << '\n';
        return 0;
    }
    return program;
}

bool ShaderProgram::reload() {
    GLuint vert = compile(GL_VERTEX_SHADER, mVertPath);
    if (!vert)
        return false;

    GLuint frag = compile(GL_FRAGMENT_SHADER, mFragPath);
    if (!frag) {
        glDeleteShader(vert);
        return false;
    }

    GLuint newProgram = link(vert, frag);

    glDeleteShader(vert);
    glDeleteShader(frag);

    if (!newProgram)
        return false;

    glDeleteProgram(mProgram);
    mProgram = newProgram;
    return true;
}

void ShaderProgram::use() const { glUseProgram(mProgram); }

GLuint ShaderProgram::getLoc(const char *name) {
    return glGetUniformLocation(mProgram, name);
}
