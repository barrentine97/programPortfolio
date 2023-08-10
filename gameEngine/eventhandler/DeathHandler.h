#pragma once
#include "EventHandler.h"
#include "../event/DeathEvent.h"
#include "../event/SpawnEvent.h"

class DeathHandler : public EventHandler {
    public:
        void onEvent(Event *event);
};