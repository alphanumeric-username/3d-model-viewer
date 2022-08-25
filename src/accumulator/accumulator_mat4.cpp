#include "accumulator/accumulator_mat4.hpp"

namespace app {

mat::mat4 AccumulatorMat4::accumulated() const {
    return accumulated_;
}

mat::mat4 AccumulatorMat4::current() const {
    return current_;
}

void AccumulatorMat4::set_current(mat::mat4 current) {
    current_ = current;
}

void AccumulatorMat4::accumulate() {
    accumulated_ = current_ * accumulated_;
}

mat::mat4 AccumulatorMat4::total() const {
    return current_ * accumulated_;
}

}