#pragma once

#include <GL/glew.h>
#include <string>

namespace Graphics {
class Shader;
enum class ShaderType {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER
};
}

namespace Graphics {
class Shader {
    ShaderType type;
    std::string name;
    std::string shader_source;
    GLuint handle = 0;
public:

    Shader(std::string  name, ShaderType type);
    ~Shader();
    Shader(const Shader& copy) = delete;
    Shader(Shader&& move) = delete;
    Shader &operator=(const Shader* assign) = delete;
    Shader &operator=(Shader &&move_assign) = delete;

    void compile();

    GLuint get_handle() const { return handle; }

    static const char* shader_path;
};
}