//
// Created by Артем on 29.09.2021.
//

#include <GL/glew.h>
#include "program.hpp"
#include "vertex-array-object-factory.hpp"
#include <iostream>

namespace Graphics {

Program::Program(): m_shaders() {}

Program::~Program() {
    for(auto shader : m_shaders) delete shader;
}

void Program::add_shader(Shader* shader) {
    m_shaders.push_back(shader);
}

void Program::link() {
    m_handle = glCreateProgram();
    GLException::check();

    for(auto shader : m_shaders) {
        if(!shader->get_handle()) throw GLException(GL_INVALID_OPERATION);
        glAttachShader(m_handle, shader->get_handle());
        GLException::check();
    }

    glLinkProgram(m_handle);
    GLException::check();

    for(auto shader : m_shaders) {
        glDetachShader(m_handle, shader->get_handle());
        GLException::check();
    }

    GLint isLinked = 0;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &maxLength);

        std::string info(maxLength, '\0');
        glGetProgramInfoLog(m_handle, maxLength, &maxLength, &info[0]);

        glDeleteProgram(m_handle);

        std::cout << info << '\n';

        throw GLProgramLinkException(info);
    }
}

void Program::set_vao(const VertexArrayObjectConfig &vao_config) {
    if(!m_handle) throw GLException(GL_INVALID_OPERATION);
    this->m_vao = vao_config.create_vao(*this);
}

GLuint Program::get_vao_handle() const {
    return this->m_vao;
}

}