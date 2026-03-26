#include "../include/Module.hpp"
#include "../include/Manager.hpp" 
#include <iostream>
#include <algorithm>

Module::Module(Manager* man, const std::string& nam)  {
    name = nam;
    manager = man;
    manager->add_module(this);

    std::cout << "Модуль создан: " << name << "\n";
}

void Module::subscribe(EventTypeID event_type){
    for(auto it : subs) {
        if (it == event_type) {
            return;
        }
    }
    subs.push_back(event_type);
    manager->add_sub_module(this, event_type);
}

void Module::unsubscribe(EventTypeID event_type) {
    for(auto it = subs.begin(); it != subs.end(); ++it) {
        if (*it == event_type) {
            subs.erase(it);
            manager->remote_sub_module(this, event_type);
            return;
        }
    }
}

void Module::deliver(const Event& event) {
    inbox_message.push(event);
}

void Module::send(EventTypeID event_type, std::string massage) {
    manager->broadcast(this, Event{event_type, massage});
}

void Module::read_messages() {
    while (!inbox_message.empty()) {
        Event ev = inbox_message.front();
        inbox_message.pop();
        std::cout << "Event_id: " << ev.type 
                  << ", message: " << ev.message << "\n";
    }
}

std::string Module::getName() {
    return name;
}