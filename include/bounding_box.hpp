#pragma once

#include <vector>

namespace app {

struct BoundingBox {
    float left{0};
    float right{0};
    float bottom{0};
    float top{0};
    float near{0};
    float far{0};
};

BoundingBox compute_bounding_box(std::vector<float> vertices);
BoundingBox compute_bounding_box(std::vector<BoundingBox> bounding_boxes);

}