#pragma once

#include "glad/glad.h"

namespace app {

void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar *message, const void *userParam);

}