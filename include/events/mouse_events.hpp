#pragma once

#include "matrix4.hpp"
#include "event_listener.hpp"
#include "bounding_box.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <array>
#include <memory>
#include <tuple>

namespace app {

class MouseListener : public EventListener {
private:
    bool needs_alt_pressed_{false};
    unsigned int mouse_state_{0};

    double left_x_acc_ {0};
    double left_x_ {0};
    double left_x0_ {0};

    double left_y_acc_ {0};
    double left_y_ {0};
    double left_y0_ {0};

    double right_x_acc_ {0};
    double right_x_ {0};
    double right_x0_ {0};

    double right_y_acc_ {0};
    double right_y_ {0};
    double right_y0_ {0};

    mat::mat4 R_acc_ { mat::identity() };
    mat::mat4 Rx_ { mat::identity() };
    mat::mat4 Ry_ { mat::identity() };
    mat::mat4 S_ { mat::identity() };

    std::array<float, 3> light_dir_{1, 1, 1};

    bool update_condition() const;
    
    void on_click(MouseClickEvent ev);
    void on_move(MouseMoveEvent ev);
    void on_scroll(MouseScrollEvent ev);

public:
    mat::mat4 S() const;

    void set_needs_alt_pressed(bool needs_alt_pressed);
    void set_S(float sx, float sy, float sz);

    double total_left_dx();
    double total_left_dy();
    double total_right_dx();
    double total_right_dy();
    mat::mat4 total_R();
    std::array<float, 3> get_light_dir();

    std::tuple<app::mat::mat4, app::mat::mat4> get_transform(app::BoundingBox model_bb);

    virtual void receive(Event event) override;
};

}