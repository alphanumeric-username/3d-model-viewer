#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace app::keyboard {

bool is_ctrl_pressed();
bool is_alt_pressed();
bool get_need_recompile_shader();
void reset_need_recompile_shader();

void on_keydown(int key, int scancode, int action, int mods);
void on_keyup(int key, int scancode, int action, int mods);

void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);

}