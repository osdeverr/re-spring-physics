//
// Created by Артем on 29.09.2021.
//

#include "error-handling.hpp"
#include <iostream>

void GLAPIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}