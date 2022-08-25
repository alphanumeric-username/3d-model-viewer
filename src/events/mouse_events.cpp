#include "events/mouse_events.hpp"
#include "events/keyboard_events.hpp"

#include "constants.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace app {

unsigned int IDLE = 0;
unsigned int READING_LEFT = 1;
unsigned int READING_RIGHT = 2;

bool MouseListener::update_condition() const {
    return needs_alt_pressed_ == keyboard::is_alt_pressed();
}

void MouseListener::on_click(MouseClickEvent ev) {
    if(ev.button == GLFW_MOUSE_BUTTON_LEFT) {
        if(ev.action == GLFW_PRESS) {
            left_x0_ = ev.xpos;
            left_y0_ = ev.ypos;

            mouse_state_ = mouse_state_ | READING_LEFT;
        } else if (ev.action == GLFW_RELEASE) {
            if(update_condition()) {
                left_x_acc_ += left_x_;
                left_y_acc_ += left_y_;

                R_acc_ = Rx_ * Ry_ * R_acc_;
            }
            
            left_x_ = 0;
            left_y_ = 0;
            
            Rx_ = mat::identity();
            Ry_ = mat::identity();

            mouse_state_ = mouse_state_ & ~READING_LEFT;
        }
    } else if (ev.button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(ev.action == GLFW_PRESS) {
            right_x0_ = ev.xpos;
            right_y0_ = ev.ypos;

            mouse_state_ = mouse_state_ | READING_RIGHT;
        } else if (ev.action == GLFW_RELEASE) {
            if(update_condition()) {
                // right_x_acc_ += right_x_;
                // right_y_acc_ += right_y_;
            }

            right_x_ = 0;
            right_y_ = 0;

            mouse_state_ = mouse_state_ & ~READING_RIGHT;
        }

    }

}

void MouseListener::on_move(MouseMoveEvent ev) {
    if((mouse_state_ & READING_LEFT) != 0) {
        left_x_ = ev.xpos - left_x0_;
        left_y_ = ev.ypos - left_y0_;
        
        if(keyboard::is_ctrl_pressed() && update_condition()) {
            std::array<float, 3> new_light_dir{
                std::clamp(2*(static_cast<float>(ev.xpos)/VP_WIDTH - 0.5f), -0.9f, 0.9f),
                std::clamp(2*(-static_cast<float>(ev.ypos)/VP_HEIGHT + 0.5f), -0.9f, 0.9f),
                0
            };

            auto radius { 
                std::max(
                    std::fabs(new_light_dir[0]), std::fabs(new_light_dir[1])
                )
            };

            auto norm {
                std::sqrt(new_light_dir[0]*new_light_dir[0] + new_light_dir[1]*new_light_dir[1])
            };

            if(norm > 0) {
                light_dir_[0] = new_light_dir[0]/norm * radius;
                light_dir_[1] = new_light_dir[1]/norm * radius;
                light_dir_[2] = std::sqrt(1 - light_dir_[0]*light_dir_[0] - light_dir_[1]*light_dir_[1]);
            } else {
                light_dir_[0] = 0;
                light_dir_[1] = 0;
                light_dir_[2] = 1;
            }

        } else {
            Rx_ = mat::rotationY(2.0f*left_x_/VP_WIDTH * PI);
            Ry_ = mat::rotationX(2.0f*left_y_/VP_HEIGHT * PI);
        }
    }
}

void MouseListener::on_scroll(MouseScrollEvent ev) {
    right_y_acc_ -= ev.yoffset;
}

double MouseListener::total_left_dx() {
    if(update_condition()) {
        return left_x_acc_ + left_x_;
    }
    return left_x_acc_;
}

double MouseListener::total_left_dy() {
    if(update_condition()) {
        return left_y_acc_ + left_y_;
    }
    return left_y_acc_;
}

double MouseListener::total_right_dx() {
    if(update_condition()) {
        return right_x_acc_ + right_x_;
    }
    return right_x_acc_;
}

double MouseListener::total_right_dy() {
    if(update_condition()) {
        return right_y_acc_ + right_y_;
    }
    return right_y_acc_;
}

mat::mat4 MouseListener::S() const {
    return S_;
}

mat::mat4 MouseListener::total_R() {
    if(update_condition()) {
        return Rx_ * Ry_ * R_acc_;
    }
    return R_acc_;
}

std::array<float, 3> MouseListener::get_light_dir() {
    return light_dir_;
}

void MouseListener::set_needs_alt_pressed(bool needs_alt_pressed) {
    needs_alt_pressed_ = needs_alt_pressed;
}

void MouseListener::set_S(float sx, float sy, float sz) {
    S_ = mat::scale(sx, sy, sz);
}

std::tuple<app::mat::mat4, app::mat::mat4> MouseListener::get_transform(app::BoundingBox model_bb) {
    auto aspect_ratio { (1.0f*VP_WIDTH)/VP_HEIGHT };
    
    auto mean_horizontal { (model_bb.left + model_bb.right)/2 };
    auto mean_vertical { (model_bb.top + model_bb.bottom)/2 };
    auto half_h_res { (model_bb.top - model_bb.bottom)*aspect_ratio/2 };

    float near{ (model_bb.top - model_bb.bottom)/(2*std::tan(FOVY/2.0f * PI/180)) };
    float far{ 100*(model_bb.far - model_bb.near ) + near };
    if(far == near) {
        far = std::max(far, near + 100);
    }

    auto Mmodel {
        app::mat::translation(
            -(model_bb.right + model_bb.left)/2,
            -(model_bb.top + model_bb.bottom)/2, 
            -(model_bb.far + model_bb.near)/2
        )
    };

    auto R { total_R() };

    auto T {
        app::mat::translation(0, 0, -near - (far - near)*total_right_dy()/VP_HEIGHT)
    };

    auto Mcenter_vp { app::mat::translation(mean_horizontal, mean_vertical, 0) };

    auto Mortho {
        app::mat::orthographic_projection(
            mean_horizontal - half_h_res, mean_horizontal + half_h_res,
            model_bb.top, model_bb.bottom, near, far
        )
    };

    
    auto P { app::mat::perspective_transform(near, far) };

    auto Mpersp {
        app::mat::perspective_projection(
            mean_horizontal - half_h_res, mean_horizontal + half_h_res,
            model_bb.top, model_bb.bottom, near, far
        )
    };

    auto Mvp = Mortho * Mcenter_vp * P;
    auto Mworld = T * R * S_ * Mmodel;

    return  std::make_tuple(Mvp, Mworld);
}

void MouseListener::receive(Event ev) {
    switch (ev.type)
    {
    case EventType::MOUSE_CLICK:
        on_click(ev.mouse_click_event);
        break;
    case EventType::MOUSE_MOVE:
        on_move(ev.mouse_move_event);
        break;
    case EventType::MOUSE_SCROLL:
        on_scroll(ev.mouse_scroll_event);
        break;
    }
}

}
