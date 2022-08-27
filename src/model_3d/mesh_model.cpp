#include "model_3d/mesh_model.hpp"

#include <iostream>

namespace app {

BoundingBox MeshModel::bounding_box() const {
    std::vector<BoundingBox> bbs{};

    for(auto& mesh : meshes_) {
        auto model_bb { compute_bounding_box(mesh.positions) };
        bbs.emplace_back(compute_bounding_box(mesh.positions));
    }

    return compute_bounding_box(bbs);
}

void MeshModel::set_meshes(std::vector<MeshData> meshes) {
    meshes_ = meshes;
}

void MeshModel::set_shader_program(ShaderProgram shader_program) {
    shader_program_ = shader_program;
}

void MeshModel::set_mouse_listener(std::shared_ptr<MouseListener> mouse_listener) {
    mouse_listener_ = mouse_listener;
}

void MeshModel::send_to_gpu() {
    shader_program_.use();

    for(auto& mesh : meshes_) {
        VertexArrayObject vao{};
        vao.bind();
        vao.set_attribute(shader_program_.get_attrib_location("a_position"), mesh.positions, 3);
        vao.set_attribute(shader_program_.get_attrib_location("a_normal"), mesh.normals, 3);
        vao.set_attribute(shader_program_.get_attrib_location("a_tex_coord"), mesh.tex_coords, 2);
        vao.unbind();

        vaos_.emplace_back(vao);

        if(mesh.material.has_map_Ka) {
            Texture map_Ka{};
            map_Ka.bind();
            map_Ka.send_texture_data(
                mesh.material.map_Ka.pixels, 
                mesh.material.map_Ka.width, 
                mesh.material.map_Ka.height
            );
            
            map_Kas_.emplace_back(map_Ka);
        } else {
            map_Kas_.emplace_back(Texture{});
        }
        
        if(mesh.material.has_map_Kd) {
            Texture map_Kd{};
            map_Kd.bind();
            map_Kd.send_texture_data(
                mesh.material.map_Kd.pixels, 
                mesh.material.map_Kd.width, 
                mesh.material.map_Kd.height
            );
            
            map_Kds_.emplace_back(map_Kd);
        } else {
            map_Kds_.emplace_back(Texture{});
        }
        
        if(mesh.material.has_map_Ks) {
            Texture map_Ks{};
            map_Ks.bind();
            map_Ks.send_texture_data(
                mesh.material.map_Ks.pixels, 
                mesh.material.map_Ks.width, 
                mesh.material.map_Ks.height
            );

            map_Kss_.emplace_back(map_Ks);
        } else {
            map_Kss_.emplace_back(Texture{});
        }

    }
}

void MeshModel::draw() const {
    glUniform1i(shader_program_.get_uniform_location("mat.map_Ka"), 0);
    glUniform1i(shader_program_.get_uniform_location("mat.map_Kd"), 1);
    glUniform1i(shader_program_.get_uniform_location("mat.map_Ks"), 2);

    auto Mvp_location { shader_program_.get_uniform_location("Mvp") };
    auto Mtrans_location { shader_program_.get_uniform_location("Mworld") };
    auto light_dir_location { shader_program_.get_uniform_location("light_dir") };
    auto [Mvp, Mworld] { mouse_listener_->get_transform(bounding_box()) };
    auto light_dir { mouse_listener_->get_light_dir() };
    
    glUniformMatrix4fv(Mvp_location, 1, false, Mvp.data());
    glUniformMatrix4fv(Mtrans_location, 1, false, Mworld.data());
    glUniform3fv(light_dir_location, 1, light_dir.data());

    for(int i = 0; i < meshes_.size(); i++) {
        auto vao {vaos_.at(i)};
        const auto& mesh { meshes_.at(i) };
        auto map_Ka { map_Kas_.at(i) };
        auto map_Kd { map_Kds_.at(i) };
        auto map_Ks { map_Kss_.at(i) };

        if(mesh.material.has_map_Ka) {
            map_Ka.assign_to_texture_unit(0);
            
            glUniform1i(
                shader_program_.get_uniform_location("mat.has_map_Ka"),
                true
            );
        } else {
            glUniform1i(
                shader_program_.get_uniform_location("mat.has_map_Ka"),
                false
            );
        }

        if(mesh.material.has_map_Kd) {
            map_Kd.assign_to_texture_unit(1);
            
            glUniform1i(
                shader_program_.get_uniform_location("mat.has_map_Kd"),
                true
            );
        } else {
            glUniform1i(
                shader_program_.get_uniform_location("mat.has_map_Kd"),
                false
            );
        }
        
        if(mesh.material.has_map_Ks) {
            map_Ks.assign_to_texture_unit(2);
            
            glUniform1i(
                shader_program_.get_uniform_location("mat.has_map_Ks"),
                true
            );
        } else {
            glUniform1i(
                shader_program_.get_uniform_location("mat.has_map_Ks"),
                false
            );
        }

        glUniform3fv(shader_program_.get_uniform_location("mat.Ka"), 1, mesh.material.Ka.data());
        glUniform3fv(shader_program_.get_uniform_location("mat.Kd"), 1, mesh.material.Kd.data());
        glUniform3fv(shader_program_.get_uniform_location("mat.Ks"), 1, mesh.material.Ks.data());
        glUniform1f(shader_program_.get_uniform_location("mat.Ns"), mesh.material.Ns);

        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0, mesh.positions.size());
    }
}

}