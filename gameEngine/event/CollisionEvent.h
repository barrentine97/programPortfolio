#pragma once
#include "Event.h"
#include "../character/Character.h"

class Character;

class CollisionEvent : public Event {
    public:
        Character *o1; // this is always the character
        GameObject *o2; // this is the object the character collides with
        std::vector<GameObject *> objects; // this is all of the objects, for side scrolling
        int deltaTime; // the offset used for side scrolling
        int direction; // 1 = left, -1 = right,
        CollisionEvent(Character *o1, GameObject *o2, std::vector<GameObject *> objects, int deltaTime);
        CollisionEvent(Character *o1, GameObject *o2, std::vector<GameObject *> objects, int deltaTime, int direction);
};