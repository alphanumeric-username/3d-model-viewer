#include "events/keyboard_events.hpp"

#include "constants.hpp"

namespace app::keyboard {

bool is_alt_pressed_ = false;
bool is_ctrl_pressed_ = false;
bool need_recompile_shader_ = false;

bool is_alt_pressed() {
    return is_alt_pressed_;
}

bool is_ctrl_pressed() {
    return is_ctrl_pressed_;
}

bool get_need_recompile_shader() {
    return need_recompile_shader_;
}

void reset_need_recompile_shader() {
    need_recompile_shader_ = false;
}

void on_keydown(int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_LEFT_CONTROL) {
        is_ctrl_pressed_ = true;
    } else if (key == GLFW_KEY_LEFT_ALT) {
        is_alt_pressed_ = true;
    }
}

void on_keyup(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F6) {
        need_recompile_shader_ = true;
    } else if (key == GLFW_KEY_LEFT_CONTROL) {
        is_ctrl_pressed_ = false;
    } else if (key == GLFW_KEY_LEFT_ALT) {
        is_alt_pressed_ = false;
    }
}

void on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if( action == GLFW_PRESS) {
        on_keydown(key, scancode, action, mods);
    } else if (action == GLFW_RELEASE) {
        on_keyup(key, scancode, action, mods);
    }
}

}