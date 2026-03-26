#pragma once
#include <string>
#include <cstdint>

using EventTypeID = uint32_t;

struct Event {
    EventTypeID type;
    std::string message;
    
    Event(EventTypeID t, const std::string& mes) : type(t), message(mes) {}
};