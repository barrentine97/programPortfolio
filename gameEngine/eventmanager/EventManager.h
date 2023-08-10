#pragma once
#include <map>
#include <vector>
#include <queue>

#include "../event/Event.h"
#include "../eventhandler/EventHandler.h"
#include "../time/Timeline.h"

class EventManager {
    private:
        EventManager() {};
        static EventManager *instance;
        static Timeline *gameTime;
        static std::map<int, std::vector<EventHandler *>> registeredHandlers;
        static std::priority_queue<Event *, std::vector<Event *>, struct compare> priorityQueue;

    public:
        static EventManager *getInstance();
        static void setTimeline(Timeline *t);
        static void registerHandler(int type, EventHandler *eventHandler);
        static void raise(Event *e);
        static void handleEvents();
};