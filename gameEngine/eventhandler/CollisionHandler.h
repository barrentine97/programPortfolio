#pragma once
#include "EventHandler.h"
#include "../event/CollisionEvent.h"
#include "../event/DeathEvent.h"

class CollisionHandler : public EventHandler {
    public:
        void onEvent(Event *event);
};