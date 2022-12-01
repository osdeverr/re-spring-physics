#pragma once

#include <string>
#include <exception>
#include <GL/glew.h>

void GLAPIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

class GLException : public std::exception {
    GLenum error;

public:
    [[nodiscard]] const char * what() const noexcept override;

    explicit GLException(GLenum error): error(error) {}

public:
    static void check() {
        GLenum error = glGetError();
        if(error) throw GLException(error);
    }

    static void enable_debug() {
//        glEnable(GL_DEBUG_OUTPUT);
//        if(glDebugMessageCallback) glDebugMessageCallback(glErrorCallback, nullptr);
    }
};

class GLStringException : std::exception {
    std::string reason;

public:
    [[nodiscard]] const char * what() const noexcept override {
        return reason.c_str();
    }

    explicit GLStringException(const std::string &reason): reason(reason) {}
};

class GLProgramLinkException : public GLStringException {
public:
    explicit GLProgramLinkException(const std::string &reason): GLStringException(reason) {}
};
class GLShaderCompileException : public GLStringException {
public:
    explicit GLShaderCompileException(const std::string &reason): GLStringException(reason) {}
};