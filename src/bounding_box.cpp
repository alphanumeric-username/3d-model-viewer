#include "bounding_box.hpp"

namespace app {

BoundingBox compute_bounding_box(std::vector<float> vertices) {
    BoundingBox bb{};

    if(vertices.size() <= 0 || vertices.size() % 3 != 0) {
        return bb;
    }

    bb.left = vertices[0];
    bb.right = vertices[0];
    bb.bottom = vertices[1];
    bb.top = vertices[1];
    bb.near = vertices[2];
    bb.far = vertices[2];

    for(int i = 0; i < vertices.size(); i+=3) {
        if(vertices[i] < bb.left) {
            bb.left = vertices[i];
        } else if (vertices[i] > bb.right) {
            bb.right = vertices[i];
        }
        
        if(vertices[i+1] < bb.bottom) {
            bb.bottom = vertices[i+1];
        } else if (vertices[i+1] > bb.top) {
            bb.top = vertices[i+1];
        }
        
        if(vertices[i+2] < bb.near) {
            bb.near = vertices[i+2];
        } else if (vertices[i+2] > bb.far) {
            bb.far = vertices[i+2];
        }
    }

    return bb;
}

BoundingBox compute_bounding_box(std::vector<BoundingBox> bounding_boxes) {
    BoundingBox bb{};

    if(bounding_boxes.size() <= 0) {
        return bb;
    }

    bb.left = bounding_boxes[0].left;
    bb.right = bounding_boxes[0].right;
    bb.bottom = bounding_boxes[0].bottom;
    bb.top = bounding_boxes[0].top;
    bb.near = bounding_boxes[0].near;
    bb.far = bounding_boxes[0].far;

    for(int i = 0; i < bounding_boxes.size(); i++) {
        if(bounding_boxes[i].left < bb.left) {
            bb.left = bounding_boxes[i].left;
        }
        if (bounding_boxes[i].right > bb.right) {
            bb.right = bounding_boxes[i].right;
        }
        if(bounding_boxes[i].bottom < bb.bottom) {
            bb.bottom = bounding_boxes[i].bottom;
        }
        if (bounding_boxes[i].top > bb.top) {
            bb.top = bounding_boxes[i].top;
        }
        if(bounding_boxes[i].near < bb.near) {
            bb.near = bounding_boxes[i].near;
        }
        if (bounding_boxes[i].far > bb.far) {
            bb.far = bounding_boxes[i].far;
        }
    }

    return bb;
}

}