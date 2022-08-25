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
    // case GL_DEBUG_SOURCE_API:
    //     std::cerr << "GL_DEBUG_SOURCE_API";
    //     break;
    // case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    //     std::cerr << "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
    //     break;
    // case GL_DEBUG_SOURCE_SHADER_COMPILER:
    //     std::cerr << "GL_DEBUG_SOURCE_SHADER_COMPILER";
    //     break;
    // case GL_DEBUG_SOURCE_THIRD_PARTY:
    //     std::cerr << "GL_DEBUG_SOURCE_THIRD_PARTY";
    //     break;
    // case GL_DEBUG_SOURCE_APPLICATION:
    //     std::cerr << "GL_DEBUG_SOURCE_APPLICATION";
    //     break;
    // case GL_DEBUG_SOURCE_OTHER:
    //     std::cerr << "GL_DEBUG_SOURCE_OTHER";
    //     break;
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
    // case GL_DEBUG_TYPE_ERROR:
    //     std::cerr << "GL_DEBUG_TYPE_ERROR";
    //     break;
    // case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    //     std::cerr << "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
    //     break;
    // case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    //     std::cerr << "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
    //     break;
    // case GL_DEBUG_TYPE_PORTABILITY:
    //     std::cerr << "GL_DEBUG_TYPE_PORTABILITY";
    //     break;
    // case GL_DEBUG_TYPE_PERFORMANCE:
    //     std::cerr << "GL_DEBUG_TYPE_PERFORMANCE";
    //     break;
    // case GL_DEBUG_TYPE_MARKER:
    //     std::cerr << "GL_DEBUG_TYPE_MARKER";
    //     break;
    // case GL_DEBUG_TYPE_PUSH_GROUP:
    //     std::cerr << "GL_DEBUG_TYPE_PUSH_GROUP";
    //     break;
    // case GL_DEBUG_TYPE_POP_GROUP:
    //     std::cerr << "GL_DEBUG_TYPE_POP_GROUP";
    //     break;
    // case GL_DEBUG_TYPE_OTHER:
    //     std::cerr << "GL_DEBUG_TYPE_OTHER";
    //     break;
    }

    std::cerr << "\n\tSeverity: ";

    switch (severity)
    {
    CASE_PRINT(GL_DEBUG_SEVERITY_HIGH);
    CASE_PRINT(GL_DEBUG_SEVERITY_MEDIUM);
    CASE_PRINT(GL_DEBUG_SEVERITY_LOW);
    CASE_PRINT(GL_DEBUG_SEVERITY_NOTIFICATION);
    // case GL_DEBUG_SEVERITY_HIGH:
    //     std::cerr << "GL_DEBUG_SEVERITY_HIGH";
    //     break;
    // case GL_DEBUG_SEVERITY_MEDIUM:
    //     std::cerr << "GL_DEBUG_SEVERITY_MEDIUM";
    //     break;
    // case GL_DEBUG_SEVERITY_LOW:
    //     std::cerr << "GL_DEBUG_SEVERITY_LOW";
    //     break;
    // case GL_DEBUG_SEVERITY_NOTIFICATION:
    //     std::cerr << "GL_DEBUG_SEVERITY_NOTIFICATION";
    //     break;
    }

    std::cerr << "\n\tMessage" << std::quoted(message) << '\n';
}

}