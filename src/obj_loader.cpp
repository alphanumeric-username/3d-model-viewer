#include "obj_loader.hpp"
#include "io.hpp"
#include "str_tools.hpp"

#include "lodepng.h"

#include <fstream>
#include <algorithm>
#include <iostream>

namespace app::obj_loader {

ObjLineTokens parse_line(std::string line) {
    // std::cout << "parse_line: Parsing line: " << std::quoted(line) << '\n';
    std::vector<std::string> words{};

    std::string current_word{""};
    for(auto c : line) {
        switch (c)
        {
        case ' ':
        case '\n':
        case '\0':
            if(current_word.length() == 0) {
                break;
            }

            // std::cout << "\t parse_line: Added word: " << std::quoted(current_word) << '\n';
            words.emplace_back(std::move(current_word));
            current_word = std::string{""};
            break;
        default:
            current_word.push_back(c);
            break;
        }
    }
    // std::cout << "\t parse_line: Added word: " << std::quoted(current_word) << '\n';
    words.emplace_back(std::move(current_word));


    ObjLineTokens tokens{};
    tokens.command = words[0];
    
    if(words.size() > 1) {
        tokens.parameters.resize(words.size() - 1);
        std::copy(words.begin()+1, words.end(), tokens.parameters.begin());
    }

    // std::cout << "\t parse_line: command: "

    return tokens;
}

FaceTuple parse_face_tuple(std::string raw) {
    std::vector<std::string> vars{
        "", "", ""
    };

    int current_var{0};
    for(auto c : raw) {
        if(c == '/') {
            current_var++;
            continue;
        }
        
        vars[current_var].push_back(c);
    }

    FaceTuple tuple{-1, -1, -1};
    if(!vars[0].empty()) {
        tuple.v = std::stoi(vars[0]) - 1;
    }
    if (!vars[1].empty()) {
        tuple.vt = std::stoi(vars[1]) - 1;
    }
    if (!vars[2].empty()) {
        tuple.vn = std::stoi(vars[2]) - 1;
    }

    return tuple;
}

void insert_vertex(std::vector<float>& list, std::vector<float>& table, int idx, int size = 3) {
    for(int i = 0; i < size; i++) {
        // std::cout << "Index: " << idx << '\n'
        //           << "\tSize: " << size << '\n'
        //           << "\ti: " << i << '\n';
        list.emplace_back(table.at(size*idx + i));
    }
}

std::vector<MeshData> load_obj(fs::path path_to_obj) {
    std::vector<MeshData> meshes{};
    std::vector<float> vertices{};
    std::vector<float> vertex_normals{};
    std::vector<float> vertex_tex_coords{};

    auto lines = read_lines(path_to_obj);
    auto root_dir { path_to_obj.parent_path() };
    std::cout << "Obj root: " << std::quoted(root_dir.string()) << '\n';

    std::map<std::string, MaterialData> material_lib{};
    MeshData current_mesh_data{};
    bool first_mesh = true;

    for(auto line : lines) {
        auto tokens = parse_line(str_tools::trim(line));
        
        if(tokens.command.compare("v") == 0) {
            vertices.emplace_back(std::stof(tokens.parameters[0]));
            vertices.emplace_back(std::stof(tokens.parameters[1]));
            vertices.emplace_back(std::stof(tokens.parameters[2]));
        } else if (tokens.command.compare("vt") == 0) {
            vertex_tex_coords.emplace_back(std::stof(tokens.parameters[0]));
            vertex_tex_coords.emplace_back(std::stof(tokens.parameters[1]));
        } else if (tokens.command.compare("vn") == 0) {
            vertex_normals.emplace_back(std::stof(tokens.parameters[0]));
            vertex_normals.emplace_back(std::stof(tokens.parameters[1]));
            vertex_normals.emplace_back(std::stof(tokens.parameters[2]));
        } else if (tokens.command.compare("f") == 0) {
            FaceTuple p0, p1, p2;

            p0 = parse_face_tuple(tokens.parameters[0]);
            p1 = parse_face_tuple(tokens.parameters[1]);
            p2 = parse_face_tuple(tokens.parameters[2]);

            if(vertices.size() > 0) {
                insert_vertex(current_mesh_data.positions, vertices, p0.v);
                insert_vertex(current_mesh_data.positions, vertices, p1.v);
                insert_vertex(current_mesh_data.positions, vertices, p2.v);
            }

            if (vertex_tex_coords.size() > 0) {
                insert_vertex(current_mesh_data.tex_coords, vertex_tex_coords, p0.vt, 2);
                insert_vertex(current_mesh_data.tex_coords, vertex_tex_coords, p1.vt, 2);
                insert_vertex(current_mesh_data.tex_coords, vertex_tex_coords, p2.vt, 2);
            }

            if (vertex_normals.size() > 0) {
                insert_vertex(current_mesh_data.normals, vertex_normals, p0.vn);
                insert_vertex(current_mesh_data.normals, vertex_normals, p1.vn);
                insert_vertex(current_mesh_data.normals, vertex_normals, p2.vn);
            }

            current_mesh_data.size++;
            
            int i;
            auto p_1 = p2;
            // auto p_2 = p1;
            for(i = 0; i < tokens.parameters.size() - 3; i++) {
                // std::cout << "Parsing parameter: " << tokens.parameters.at(3 + i) << '\n';
                // std::cout << "\tParsing parameter: parameters.size(): " << tokens.parameters.size() << '\n';
                // std::cout << "\tParsing parameter: i: " << i << '\n';
                auto p = parse_face_tuple(tokens.parameters.at(3 + i));

                if(vertices.size() > 0) {
                    // std::cout << "Inserting positions\n";
                    insert_vertex(current_mesh_data.positions, vertices, p0.v);
                    insert_vertex(current_mesh_data.positions, vertices, p_1.v);
                    insert_vertex(current_mesh_data.positions, vertices, p.v);
                }
                
                if(vertex_tex_coords.size() > 0) {
                    // std::cout << "Inserting texture coordinates\n";
                    insert_vertex(current_mesh_data.tex_coords, vertex_tex_coords, p0.vt, 2);
                    insert_vertex(current_mesh_data.tex_coords, vertex_tex_coords, p_1.vt, 2);
                    insert_vertex(current_mesh_data.tex_coords, vertex_tex_coords, p.vt, 2);
                }

                if(vertex_normals.size() > 0) {
                    // std::cout << "Inserting normals\n";
                    insert_vertex(current_mesh_data.normals, vertex_normals, p0.vn);
                    insert_vertex(current_mesh_data.normals, vertex_normals, p_1.vn);
                    insert_vertex(current_mesh_data.normals, vertex_normals, p.vn);
                }

                p_1 = p;
            }

            current_mesh_data.size += i;
        } else if (tokens.command.compare("g") == 0 || tokens.command.compare("o") == 0) {
            if(first_mesh) {
                first_mesh = false;
            } else {
                meshes.emplace_back(current_mesh_data);
                current_mesh_data = MeshData{};
            }

            current_mesh_data.name = tokens.parameters[0];
            // std::cout << current_mesh_data.name << '\n';
        } else if (tokens.command.compare("mtllib") == 0) {
            material_lib = load_mtl(root_dir / tokens.parameters[0]);
        } else if (tokens.command.compare("usemtl") == 0) {
            current_mesh_data.has_material = true;
            current_mesh_data.material = material_lib.at(tokens.parameters[0]);
        }
    }

    if(!first_mesh) {
        meshes.emplace_back(current_mesh_data);
    }

    return meshes;
}

std::map<std::string, MaterialData> load_mtl(fs::path path_to_mtl) {
    auto lines { read_lines(path_to_mtl) };
    auto root_dir { path_to_mtl.parent_path() };

    std::map<std::string, MaterialData> material_lib{};
    std::string current_mat_key{""};

    for(auto line : lines) {
        auto tokens = parse_line(str_tools::trim(line));

        if(tokens.command.compare("newmtl") == 0) {
            current_mat_key = tokens.parameters[0];
            material_lib.insert({current_mat_key, MaterialData{}});
        } else if(tokens.command.compare("Ka") == 0) {
            material_lib.at(current_mat_key).Ka = {
                std::stof(tokens.parameters[0]),
                std::stof(tokens.parameters[1]),
                std::stof(tokens.parameters[2])
            };
        } else if(tokens.command.compare("Kd") == 0) {
            material_lib.at(current_mat_key).Kd = {
                std::stof(tokens.parameters[0]),
                std::stof(tokens.parameters[1]),
                std::stof(tokens.parameters[2])
            };
        } else if(tokens.command.compare("Ks") == 0) {
            material_lib.at(current_mat_key).Ks = {
                std::stof(tokens.parameters[0]),
                std::stof(tokens.parameters[1]),
                std::stof(tokens.parameters[2])
            };
        } else if (tokens.command.compare("map_Ka") == 0) {
            std::vector<unsigned char> container{};
            unsigned int w, h;
            auto filepath { root_dir / tokens.parameters[0] };
            auto err = lodepng::decode(container, w, h, filepath.string());
            if(err) {
                std::cerr << "[LODEPNG]: " << lodepng_error_text(err);
            }
            material_lib.at(current_mat_key).map_Ka = { w, h, container};
            material_lib.at(current_mat_key).has_map_Ka = true;
        
        } else if (tokens.command.compare("map_Kd") == 0) {
            std::vector<unsigned char> container{};
            unsigned int w, h;
            auto filepath { root_dir / tokens.parameters[0] };
            auto err = lodepng::decode(container, w, h, filepath.string());
            if(err) {
                std::cerr << "[LODEPNG]: " << lodepng_error_text(err);
            }
            material_lib.at(current_mat_key).map_Kd = { w, h, container};
            material_lib.at(current_mat_key).has_map_Kd = true;
        
        } else if (tokens.command.compare("map_Ks") == 0) {
            std::vector<unsigned char> container{};
            unsigned int w, h;
            auto filepath { root_dir / tokens.parameters[0] };
            auto err = lodepng::decode(container, w, h, filepath.string());
            if(err) {
                std::cerr << "[LODEPNG]: " << lodepng_error_text(err);
            }
            material_lib.at(current_mat_key).map_Ks = { w, h, container};
            material_lib.at(current_mat_key).has_map_Ks = true;
        
        } else if(tokens.command.compare("Ns") == 0) {
            material_lib.at(current_mat_key).Ns = std::stof(tokens.parameters[0]);
        }
    }

    return material_lib;
}

};