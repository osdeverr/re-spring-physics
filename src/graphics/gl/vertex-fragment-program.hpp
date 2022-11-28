#pragma once

namespace Graphics {
struct VertexArrayObjectConfig;
struct VertexArrayObjectEntryConfig;
struct VertexAttributeConfig;
}

#include "program.hpp"
#include "gl-buffer.hpp"

namespace Graphics {
class VertexFragmentProgram : public Program {
public:
    VertexFragmentProgram(const std::string& vertex_shader_name, const std::string& fragment_shader_name) {
        auto vertex_shader = new Shader(vertex_shader_name, ShaderType::vertex);
        auto fragment_shader = new Shader(fragment_shader_name, ShaderType::fragment);

        vertex_shader->compile();
        fragment_shader->compile();

        add_shader(vertex_shader);
        add_shader(fragment_shader);

        link();
    }
};
}