#pragma once

namespace Graphics {
class Program;
}

#include <vector>
#include "shader.hpp"
#include "gl-buffer.hpp"

namespace Graphics {
struct VertexArrayObjectConfig;

class Program {
protected:
    std::vector<Shader*> m_shaders {};
    GLuint m_vao = 0;
    GLuint m_handle = 0;

protected:
    void bind_vao() { glBindVertexArray(m_vao); GLException::check(); }
    void unbind_vao() { glBindVertexArray(0); }
public:
    Program();
    virtual ~Program();
    Program(const Program& copy) = delete;
    Program(Program&& move) = delete;
    Program &operator=(const Program* assign) = delete;
    Program &operator=(Program &&move_assign) = delete;

    void add_shader(Shader* shader);
    void set_vao(const VertexArrayObjectConfig& vao_config);
    GLuint get_attribute_index(const std::string& str) const {
        GLuint loc = glGetAttribLocation(m_handle, str.c_str());
        GLException::check();
        return loc;
    }

    void link();

    GLuint get_handle() const { return m_handle; }
    GLuint get_vao_handle() const;

    void use() { glUseProgram(get_handle()); GLException::check(); }
};
}