//
// Created by Артем on 29.09.2021.
//

#include "shader.hpp"
#include "../../error-handling.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>

namespace Graphics {

const char* Shader::shader_path = "resources/shaders/";

Shader::Shader(std::string  name, ShaderType type): type(type), name(std::move(name)) {}

Shader::~Shader() {
    if(handle) glDeleteShader(handle);
}

void Shader::compile() {
    std::stringstream shader_source_path;
    shader_source_path << shader_path << name << ".shader";
    std::string shader_path = shader_source_path.str();
    std::ifstream shader_source_fstream(shader_path);

    std::stringstream shader_source_stream;
    shader_source_stream << shader_source_fstream.rdbuf() << '\0';
    shader_source_fstream.close();

    shader_source = shader_source_stream.str();

    const GLint lengths[] = { (GLint)shader_source.size() };
    const char* sources[] = { shader_source.c_str() };

    handle = glCreateShader((GLenum)type);
    GLException::check();

    glShaderSource(handle, 1, sources, lengths);
    GLException::check();

    glCompileShader(handle);
    GLException::check();

    GLint isCompiled = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);

    GLint maxLength = 0;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

    std::string info;

    if(maxLength > 0) {
        info.resize(maxLength);
        glGetShaderInfoLog(handle, maxLength, &maxLength, &info[0]);
        while(info[info.size() - 1] == '\0') info.pop_back();
        std::cout << shader_path << ":\n" << info << "\n";
    }

    if(isCompiled == GL_FALSE) {
        glDeleteShader(handle);
        handle = 0;

        if(info.empty()) info = "Unknown Error";
        throw GLShaderCompileException(info);
    }
}
}