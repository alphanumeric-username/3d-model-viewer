#include "accumulator.hpp"

#include "matrix4.hpp"

namespace app {

class AccumulatorMat4 {
private:
    mat::mat4 accumulated_ {mat::identity()};
    mat::mat4 current_ {mat::identity()};

public:
    mat::mat4 accumulated() const;
    mat::mat4 current() const;

    void set_current(mat::mat4 current);
    void accumulate();

    mat::mat4 total() const;
};

}