#include "EventManager.h"

/**
 * @brief private helper function to organize priority queue based on timeline
 * 
 */
struct compare {
    bool operator() (Event *e1, Event *e2) {
        return (e1->timestamp) > (e2->timestamp);
    }
};

/** the event manager instance */
EventManager *EventManager::instance;
/** the timeline used for timestamps */
Timeline *EventManager::gameTime;
/** list of registered handlers, registered to their appropriate event types */
std::map<int, std::vector<EventHandler *>> EventManager::registeredHandlers;
/** list of events, organized in a priority queue*/
std::priority_queue<Event *, std::vector<Event *>, compare> EventManager::priorityQueue;

/**
 * @brief returns the singleton instance of event manager
 * 
 * @return EventManager* 
 */
EventManager *EventManager::getInstance() {
    if (!instance) {
        instance = new EventManager();
    }

    return instance;
}

/**
 * @brief sets the timeline to the given timeline
 * 
 * @param t the timeline to set
 */
void EventManager::setTimeline(Timeline *t) {
    gameTime = t;
}

/**
 * @brief registers the given handler with the given event type
 * 
 * @param type the type to associate with the event handler
 * @param eventHandler the event handler to register
 */
void EventManager::registerHandler(int type, EventHandler *eventHandler) {
    std::vector<EventHandler *> list;
    if (registeredHandlers.find(type) != registeredHandlers.end()) {
        list = registeredHandlers.at(type);
    }
    list.push_back(eventHandler);
    registeredHandlers.insert(std::pair<int, std::vector<EventHandler *>>(type, list));
}

/**
 * @brief raises an event
 * 
 * @param e the event to raise
 */
void EventManager::raise(Event *e) {
    e->setTimestamp(gameTime->getTime());
    priorityQueue.push(e);
}

/**
 * @brief handles all events in the priority queue
 */
void EventManager::handleEvents() {
    while (!priorityQueue.empty()) {
        Event *e = priorityQueue.top();
        std::vector<EventHandler *> handlers = registeredHandlers[e->type];
        for (int i = 0; i < handlers.size(); i++) {
            handlers[i]->onEvent(e);
        }

        priorityQueue.pop();
    }
}