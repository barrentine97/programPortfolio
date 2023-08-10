#include "CollisionEvent.h"

/**
 * @brief Construct a new Collision Event:: Collision Event object
 * 
 * @param o1 first object in the collision
 * @param o2 second object in the collison
 * @param objects list of all game objects
 * @param deltaTime deltaTime for the frame
 */
CollisionEvent::CollisionEvent(Character *o1, GameObject *o2, std::vector<GameObject *> objects, int deltaTime) {
    type = TYPE_COLLISION;
    this->o1 = o1;
    this->o2 = o2;
    this->objects = objects;
    this->deltaTime = deltaTime;
    this->direction = 0;
}

/**
 * @brief Construct a new Collision Event:: Collision Event object
 * 
 * @param o1 first object in the collision
 * @param o2 second object in the collison
 * @param objects list of all game objects
 * @param deltaTime deltaTime for the frame
 * @param direction the direction the character should be moving
 */
CollisionEvent::CollisionEvent(Character *o1, GameObject *o2, std::vector<GameObject *> objects, int deltaTime, int direction) {
    type = TYPE_COLLISION;
    this->o1 = o1;
    this->o2 = o2;
    this->objects = objects;
    this->deltaTime = deltaTime;
    this->direction = direction;
}