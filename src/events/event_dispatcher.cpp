#include "events/event_dispatcher.hpp"

namespace app {

void EventDispatcher::subscribe(std::shared_ptr<EventListener> listener, EventType type_channel) {
    if(listeners_.count(type_channel) == 0) {
        listeners_.insert_or_assign(type_channel, std::vector<std::shared_ptr<EventListener>> {});
    }

    listeners_.at(type_channel).emplace_back(listener);
}

void EventDispatcher::unsubscribe(std::shared_ptr<EventListener> listener, EventType type_channel) {
    if(listeners_.count(type_channel) == 0) {
        return;
    }

    auto& channel_listeners { listeners_.at(type_channel) };
    std::vector<std::shared_ptr<EventListener>> new_channel_listeners{};

    for(auto l : channel_listeners) {
        if(l != listener) {
            new_channel_listeners.emplace_back(l);
        }
    }

    listeners_.at(type_channel) = new_channel_listeners;
}

void EventDispatcher::on_mouse_move(GLFWwindow* win, double xpos, double ypos) {
    Event event{};
    event.type = EventType::MOUSE_MOVE;
    event.mouse_move_event = { xpos, ypos };

    for(auto listener : listeners_.at(EventType::MOUSE_MOVE)) {
        listener->receive(event);
    }
}

void EventDispatcher::on_mouse_click(GLFWwindow* win, int button, int action, int mods) {
    Event event{};
    event.type = EventType::MOUSE_CLICK;
    event.mouse_click_event = { button, action, mods };

    glfwGetCursorPos(win, &(event.mouse_click_event.xpos), &(event.mouse_click_event.ypos));

    for(auto listener : listeners_.at(EventType::MOUSE_CLICK)) {
        listener->receive(event);
    }
}

void EventDispatcher::on_mouse_scroll(GLFWwindow* win, double xoffset, double yoffset) {
    Event event{};
    event.type = EventType::MOUSE_SCROLL;
    event.mouse_scroll_event = { xoffset, yoffset };

    for(auto listener : listeners_.at(EventType::MOUSE_SCROLL)) {
        listener->receive(event);
    }
}

void EventDispatcher::on_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Event event{};
    event.type = EventType::KEYBOARD;
    event.keyboard_event = { key, scancode, action, mods };

    for(auto listener : listeners_.at(EventType::KEYBOARD)) {
        listener->receive(event);
    }
}

}