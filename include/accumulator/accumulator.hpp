#pragma once

namespace app {

template<typename T>
class Accumulator {
private:
    T accumulated_;
    T current_;

public:
    Accumulator();

    T accumulated() const {
        return accumulated_;
    }

    T current() const {
        return current_;
    }

    void set_current(T current) {
        current_ = current;
    }

    void accumulate() {
        accumulated_ = accumulated_ + current_;
    }

    T total() const {
        return accumulated_ + current_;
    }
};

}