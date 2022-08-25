#pragma once

#include <array>
#include <filesystem>
#include <vector>
#include <map>

namespace app {

namespace fs = std::filesystem;

struct TextureData {
    unsigned int width;
    unsigned int height;
    std::vector<unsigned char> pixels{};
};

struct MaterialData {
    std::array<float, 3> Ka;
    std::array<float, 3> Kd;
    std::array<float, 3> Ks;
    float Ns;

    TextureData map_Ka{};
    TextureData map_Kd{};
    TextureData map_Ks{};

    bool has_map_Ka{false};
    bool has_map_Kd{false};
    bool has_map_Ks{false};
};

struct MeshData {
    std::string name;

    std::vector<float> positions{};
    std::vector<float> normals{};
    std::vector<float> tex_coords{};
    int size{0};

    bool has_material = false;
    MaterialData material;
};

struct ObjLineTokens {
    std::string command;
    std::vector<std::string> parameters{};
};

struct FaceTuple {
    int v;
    int vt;
    int vn;
};

namespace obj_loader {
    std::vector<MeshData> load_obj(fs::path path_to_obj);
    std::map<std::string, MaterialData> load_mtl(fs::path path_to_mtl);
}

};