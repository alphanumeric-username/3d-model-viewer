#pragma once

#include "event.hpp"

namespace app {

class EventListener {
public:
    virtual void receive(Event event) = 0;
    // void operator()(Event event);
};

}