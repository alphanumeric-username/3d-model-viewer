#pragma once

#include "opengl_wrapper/vertex_array_object.hpp"
#include "opengl_wrapper/shader_program.hpp"
#include "opengl_wrapper/texture.hpp"
#include "bounding_box.hpp"
#include "obj_loader.hpp"
#include "matrix4.hpp"
#include "events/mouse_events.hpp"

#include <vector>
#include <memory>

namespace app {

class MeshModel {
private:
    std::vector<VertexArrayObject> vaos_{};
    std::vector<MeshData> meshes_{};
    std::vector<Texture> map_Kas_{};
    std::vector<Texture> map_Kds_{};
    std::vector<Texture> map_Kss_{};
    ShaderProgram shader_program_;
    std::shared_ptr<MouseListener> mouse_listener_{nullptr};


public:
    BoundingBox bounding_box() const;

    void set_meshes(std::vector<MeshData> meshes);
    void set_shader_program(ShaderProgram shader_program);
    void set_mouse_listener(std::shared_ptr<MouseListener> mouse_listener);

    void send_to_gpu();
    void draw() const;
};

}