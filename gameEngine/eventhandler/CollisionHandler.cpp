#include "CollisionHandler.h"

/**
 * @brief function that handles a collision event
 * 
 * @param event the collision event to handle
 */
void CollisionHandler::onEvent(Event *event) {
    auto *e = static_cast<CollisionEvent *>(event);
{
    // if the object is a deathzone, the character dies
    auto obj = dynamic_cast<DeathZone *>(e->o2);
    if (obj != nullptr) {
        DeathEvent *de = new DeathEvent(e->o1);
        EventManager::raise(de);
    }
}
{
    // if the object is a side boundary, everything visible shifts right
    auto obj = dynamic_cast<SideBoundary *>(e->o2);
    if (obj != nullptr) {
        for (int i = 0; i < e->objects.size(); i++) {
            if (e->objects[i]->getObjectId() != e->o1->getObjectId()) {
                e->objects[i]->slideOver(e->direction * e->deltaTime * e->o1->getVelocityX());
            }
        }
    }
}

    //otherwise, fix the overlap
    sf::FloatRect charBox = e->o1->getGlobalBounds();
    float leftOfChar = charBox.left;
    float rightOfChar = leftOfChar + charBox.width;
    float topOfChar = charBox.top;
    float bottomOfChar = topOfChar + charBox.height;

    auto obj = dynamic_cast<sf::Shape *>(e->o2);
    sf::FloatRect objBox = obj->getGlobalBounds();
    float leftOfObj = objBox.left;
    float rightOfObj = leftOfObj + objBox.width;
    float topOfObj = objBox.top;
    float bottomOfObj = topOfObj + objBox.height;

    // if moving left, fix left overlap
    if (e->direction == 1) {
        if (leftOfChar < rightOfObj && rightOfChar > rightOfObj) {
            e->o1->move(rightOfObj - leftOfChar, 0);
        }
    }

    // if moving right, fix right overlap
    if (e->direction == -1) {
        if (rightOfChar > leftOfObj && leftOfChar < leftOfObj) {
            e->o1->move(leftOfObj - rightOfChar, 0);
        }
    }

    // if moving vertically, fix the overlap
    if (topOfChar < bottomOfObj && bottomOfChar > bottomOfObj &&
        leftOfChar != rightOfObj && rightOfChar !=leftOfObj) {
        // we've hit the bottom of an object
        e->o1->move(0, bottomOfObj - topOfChar);

    } else if (bottomOfChar > topOfObj && topOfChar < topOfObj &&
            leftOfChar != rightOfObj && rightOfChar != leftOfObj) {
        // we've hit the top of an object. We are now on the ground
        e->o1->setOnGround(true);
        e->o1->setVelocityY(0);
        e->o1->move(0, topOfObj - bottomOfChar);
    }

    // if on a horizontal moving platform, move with it
    auto mp = dynamic_cast<MovingPlatform *>(e->o2);
    if (mp != nullptr) {
        if (mp->getDirection() == 2) {
            e->o1->move(mp->getVelocity() * e->deltaTime, 0);
        } else if (mp->getDirection() == 4) {
            e->o1->move(-1 * mp->getVelocity() * e->deltaTime, 0);
        }
    }
}