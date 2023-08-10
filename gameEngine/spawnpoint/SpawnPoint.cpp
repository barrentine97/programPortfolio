#include <iostream>
#include "SpawnPoint.h"

/**
 * @brief Construct a new Spawn Point:: Spawn Point object
 * 
 * @param x coordinate of the spawnPoint
 * @param y coordinate of the spawnPoint
 */
SpawnPoint::SpawnPoint(int id, int x, int y) {
    setObjectId(id);
    this->positionX = x;
    this->positionY = y;
    setPosition(x, y);
    setRadius(10);
    setFillColor(sf::Color::Green);
}

/**
 * @brief spawns the given character at the location of the spawnpoint
 * 
 * @param c the character to spawn
 */
void SpawnPoint::spawn(Character *c) {
    c->setPosition(positionX, positionY);
    c->setDead(false);
    c->setVelocityY(0);
}

/**
 * @brief returns a json representation of the object
 * 
 * @param index the index of the object in the list of objects
 * @return json the json representation of the object
 */
json SpawnPoint::getJson(int index) {
    json toReturn;
    toReturn[std::to_string(index)]["type"] = "SpawnPoint";
    toReturn[std::to_string(index)]["id"] = getObjectId();
    toReturn[std::to_string(index)]["positionX"] = getPosition().x;
    toReturn[std::to_string(index)]["positionY"] = getPosition().y;
    return toReturn;
}