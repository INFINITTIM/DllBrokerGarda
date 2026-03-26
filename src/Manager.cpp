#include "../include/Manager.hpp"
#include "../include/Module.hpp"
#include <algorithm> 

void Manager::add_module(Module* module) {
    modules.push_back(module);
}

void Manager::add_sub_module(Module* m, EventTypeID type) {
    subscribers[type].push_back(m);
}

void Manager::remote_sub_module(Module* module, EventTypeID type) {
    auto it = subscribers.find(type);
    if (it != subscribers.end()) {
        auto& vec = it->second;
        vec.erase(std::remove(vec.begin(), vec.end(), module), vec.end());
    }
}

void Manager::broadcast(Module* sender, const Event& event) {
    auto it = subscribers.find(event.type);
    if (it != subscribers.end()) {
        for (auto* sub : it->second) {
            if (sub != sender) {
                sub->deliver(event);
            }
        }
    }
}