#pragma once

namespace app {

enum EventType { MOUSE_MOVE, MOUSE_CLICK, MOUSE_SCROLL, KEYBOARD };

struct MouseMoveEvent {
    double xpos;
    double ypos;
};

struct MouseClickEvent {
    int button;
    int action;
    int mods;
    double xpos;
    double ypos;
};

struct MouseScrollEvent {
    double xoffset;
    double yoffset;
};

struct KeyboardEvent {
    int key;
    int scancode;
    int action;
    int mods;
};

struct Event {
    EventType type;
    MouseMoveEvent mouse_move_event;
    MouseClickEvent mouse_click_event;
    KeyboardEvent keyboard_event;
    MouseScrollEvent mouse_scroll_event;
};

}