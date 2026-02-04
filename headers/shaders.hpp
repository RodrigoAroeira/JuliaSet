#include <glad/glad.h>

#include <string>

class ShaderProgram {
  public:
    ShaderProgram(std::string vertPath, std::string fragPath);
    ~ShaderProgram();

    bool reload();
    void use() const;

    GLuint id() const { return mProgram; };
    GLuint getLoc(const char *name);

    ShaderProgram &operator=(ShaderProgram &&) noexcept;

  private:
    GLuint mProgram;
    std::string mVertPath;
    std::string mFragPath;

    GLuint compile(GLenum type, const std::string &path);
    GLuint link(GLuint vert, GLuint frag);
};
