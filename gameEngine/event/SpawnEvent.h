#pragma once
#include "Event.h"
#include "../character/Character.h"
#include "../spawnpoint/SpawnPoint.h"

class Character;

class SpawnEvent : public Event {
    public:
        SpawnPoint *spawnPoint;
        Character *character;
        SpawnEvent(Character *character);
};