#include "opengl_wrapper/message_callback.hpp"

#include <iostream>
#include <iomanip>

namespace app {

#define CASE_PRINT(x) case x: std::cerr << #x; break;

void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar *message, const void *userParam) {
    std::cerr << "[OPEN GL]: Source: ";
    switch (source)
    {
    CASE_PRINT(GL_DEBUG_SOURCE_API);
    CASE_PRINT(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
    CASE_PRINT(GL_DEBUG_SOURCE_SHADER_COMPILER);
    CASE_PRINT(GL_DEBUG_SOURCE_THIRD_PARTY);
    CASE_PRINT(GL_DEBUG_SOURCE_APPLICATION);
    CASE_PRINT(GL_DEBUG_SOURCE_OTHER);
    }

    std::cerr << "\n\tType: ";

    switch (type)
    {
    CASE_PRINT(GL_DEBUG_TYPE_ERROR);
    CASE_PRINT(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
    CASE_PRINT(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
    CASE_PRINT(GL_DEBUG_TYPE_PORTABILITY);
    CASE_PRINT(GL_DEBUG_TYPE_PERFORMANCE);
    CASE_PRINT(GL_DEBUG_TYPE_MARKER);
    CASE_PRINT(GL_DEBUG_TYPE_PUSH_GROUP);
    CASE_PRINT(GL_DEBUG_TYPE_POP_GROUP);
    CASE_PRINT(GL_DEBUG_TYPE_OTHER);
    }

    std::cerr << "\n\tSeverity: ";

    switch (severity)
    {
    CASE_PRINT(GL_DEBUG_SEVERITY_HIGH);
    CASE_PRINT(GL_DEBUG_SEVERITY_MEDIUM);
    CASE_PRINT(GL_DEBUG_SEVERITY_LOW);
    CASE_PRINT(GL_DEBUG_SEVERITY_NOTIFICATION);
    }

    std::cerr << "\n\tMessage" << std::quoted(message) << '\n';
}

}