#pragma once
#include <string>
#include <vector>
#include <queue>

#include "Event.hpp"

class Manager;

class Module{
 private:
   std::string name;
   std::vector<EventTypeID> subs;
   Manager* manager;
 protected:
   std::queue<Event> inbox_message; 
 public:

   Module(Manager* man, const std::string& nam);

   void subscribe(EventTypeID event_type);

   void unsubscribe(EventTypeID event_type);

   void deliver(const Event& event);

   void send(EventTypeID event_type, std::string massage);

   void read_messages();

   std::string getName();
};
