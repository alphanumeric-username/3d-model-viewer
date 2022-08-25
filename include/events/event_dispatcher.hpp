#pragma once

#include "event.hpp"
#include "event_listener.hpp"

#include <GLFW/glfw3.h>

#include <vector>
#include <map>
#include <memory>

namespace app {

class EventDispatcher {
private:
    std::map<EventType, std::vector<std::shared_ptr<EventListener>>> listeners_{};

public:
    void subscribe(std::shared_ptr<EventListener> listener, EventType type_channel);
    void unsubscribe(std::shared_ptr<EventListener> listener, EventType type_channel);

    void on_mouse_click(GLFWwindow* win, int button, int action, int mods);
    void on_mouse_move(GLFWwindow* win, double xpos, double ypos);
    void on_mouse_scroll(GLFWwindow* win, double xoffset, double yoffset);
    void on_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
};

}