#include "matrix4.hpp"

#include <cmath>
#include <iostream>

namespace app::mat {

mat4 identity() {
    return mat4 {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

mat4 mult(mat4 A, mat4 B) {
    mat4 result{};

    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            result[col*4 + row] = A[0*4 + row]*B[col*4 + 0] +
                                  A[1*4 + row]*B[col*4 + 1] +
                                  A[2*4 + row]*B[col*4 + 2] +
                                  A[3*4 + row]*B[col*4 + 3];
        }
    }

    return result;
}

mat4 translation(float dx, float dy, float dz) {
    return mat4 {
        1,  0,  0,  0,
        0,  1,  0,  0,
        0,  0,  1,  0,
        dx, dy, dz, 1,
    };
}

mat4 scale(float sx, float sy, float sz) {
    return mat4 {
        sx, 0,  0,  0,
        0,  sy, 0,  0,
        0,  0,  sz, 0,
        0,  0,  0,  1,
    };
}

mat4 scale(float s) {
    return scale(s, s, s);
}

mat4 rotationX(float t) {
    auto ct { std::cos(t) };
    auto st { std::sin(t) };

    return mat4 {
        1,   0,  0, 0,
        0,  ct, st, 0,
        0, -st, ct, 0,
        0,   0,  0, 1,
    };
}

mat4 rotationY(float t) {
    auto ct { std::cos(t) };
    auto st { std::sin(t) };

    return mat4 {
        ct, 0, -st, 0,
         0, 1,   0, 0,
        st, 0,  ct, 0,
         0, 0,   0, 1,
    };
}

mat4 rotationZ(float t) {
    auto ct { std::cos(t) };
    auto st { std::sin(t) };

    return mat4 {
        ct, st, 0, 0,
       -st, ct, 0, 0,
         0,  0, 1, 0,
         0,  0, 0, 1
    };
}

mat4 orthographic_projection(float l, float r, float t, float b, float n, float f) {
    return translation(0, 0, 0.5f) * scale(2/(r-l), 2/(t-b), 1/(n - f)) * translation(-(r+l)/2, -(t+b)/2, (n+f)/2);
}

mat4 perspective_projection(float l, float r, float t, float b, float n, float f) {
    mat4 P {
        n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n+f, -1,
        0, 0, f*n, 0,
    };

    return orthographic_projection(l, r, t, b, n, f) * perspective_transform(n, f);
}

mat4 perspective_transform(float n, float f) {
    return mat4 {
        n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n+f, -1,
        0, 0, f*n, 0,
    };
}

float at(mat4& M, int i, int j) {
    return M[i + 4*j];
}

void print_matrix(mat4 M) {
    for (int i = 0; i < 4; i++) {
        std::cout << "[ ";
        for (int j = 0; j < 4; j++) {
            std::cout << at(M, i, j) << ' ';
        }
        std::cout << "]\n";
    }
    
}

}

app::mat::mat4 operator*(app::mat::mat4 A, app::mat::mat4 B) {
    return app::mat::mult(A, B);
}