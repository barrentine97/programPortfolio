#pragma once
#include "Event.h"
#include "../character/Character.h"

class Character;

class InputEvent : public Event {
    public:
        Character *character;
        bool left;
        bool right;
        bool up;
        InputEvent(Character *character, bool left, bool right, bool up);
};