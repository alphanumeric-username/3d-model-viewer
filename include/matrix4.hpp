#pragma once

#include <array>

namespace app::mat {

typedef std::array<float, 16> mat4;

mat4 identity();
mat4 mult(mat4 A, mat4 B);

mat4 translation(float dx, float dy, float dz);
mat4 scale(float sx, float sy, float sz);
mat4 scale(float s);
mat4 rotationX(float t);
mat4 rotationY(float t);
mat4 rotationZ(float t);
mat4 orthographic_projection(float l, float r, float t, float b, float n, float f);
mat4 perspective_projection(float l, float r, float t, float b, float n, float f);
mat4 perspective_transform(float n, float f);
float at(mat4& M, int i, int j);

void print_matrix(mat4 M);

}

app::mat::mat4 operator*(app::mat::mat4 A, app::mat::mat4 B);