#pragma once

#include "opengl_wrapper/texture.hpp"

#include "glad/glad.h"

namespace app {

class Framebuffer {
private:
    GLuint handle_{0};
    GLuint depth_buffer_{0};
    std::vector<GLenum> color_attachments_{};
    std::vector<Texture> attached_textures_{};
    GLint original_handle_{0};

public:
    GLint handle() const;

    void bind();
    void unbind();
    void attach_depth_buffer(GLuint width, GLuint height);
    void attach_texture(Texture texture);
    void setup_draw_buffers();
    void update_mipmaps();
    void begin_draw();
    void end_draw();


};

}