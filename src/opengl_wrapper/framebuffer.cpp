#include "opengl_wrapper/framebuffer.hpp"

namespace app {

GLint Framebuffer::handle() const {
    return handle_;
}

void Framebuffer::bind() {
    if(handle_ == 0) {
        glGenFramebuffers(1, &handle_);
    }

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &original_handle_);
    glBindFramebuffer(GL_FRAMEBUFFER, handle_);
}

void Framebuffer::attach_depth_buffer(GLuint width, GLuint height) {
    glGenRenderbuffers(1, &depth_buffer_);

    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_);
}

void Framebuffer::attach_texture(Texture texture) {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + color_attachments_.size(), texture.handle(), 0);
    color_attachments_.emplace_back(GL_COLOR_ATTACHMENT0 + color_attachments_.size());
    attached_textures_.emplace_back(texture);
}

void Framebuffer::setup_draw_buffers() {
    glDrawBuffers(color_attachments_.size(), color_attachments_.data());
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, original_handle_);
    original_handle_ = 0;
}

void Framebuffer::update_mipmaps() {
    for(auto& texture : attached_textures_) {
        texture.bind();
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

}