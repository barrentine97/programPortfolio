#pragma once
#include "EventHandler.h"
#include "../event/SpawnEvent.h"

class SpawnHandler : public EventHandler {
    private:
        SpawnPoint *spawnPoint;
        
    public:
        SpawnHandler(SpawnPoint *spawnPoint);
        void onEvent(Event *event);
};