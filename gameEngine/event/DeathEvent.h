#pragma once
#include "Event.h"
#include "../character/Character.h"

class Character;

class DeathEvent : public Event {
    public:
        Character *character;
        DeathEvent(Character *character);
};