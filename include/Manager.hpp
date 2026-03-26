#pragma once
#include <string>
#include <vector>
#include <map>

#include "Event.hpp"

class Module;

class Manager{
 private: 
    std::vector<Module*> modules;
    std::map<uint32_t, std::vector<Module*>> subscribers;
 public:
    void broadcast(Module* sender, const Event& event);

    void add_module(Module* module);

    void add_sub_module(Module* m, EventTypeID type);

    void remote_sub_module(Module* module, EventTypeID type);
};
