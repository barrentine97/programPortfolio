#include "DeathZone.h"

/**
 * @brief Construct a new Death Zone:: Death Zone object
 * 
 * @param id the object id
 */
DeathZone::DeathZone(int id) {
    setObjectId(id);
}

/**
 * @brief overrides the abstract class to make this object collidable
 * 
 * @return true since this object is collidable
 */
bool DeathZone::isCollidable() {
    return true;
}

/**
 * @brief returns a json representation of the object
 * 
 * @param index the index of the object in the list of objects
 * @return json the json representation of the object
 */
json DeathZone::getJson(int index) {
    json toReturn;
    toReturn[std::to_string(index)]["type"] = "DeathZone";
    toReturn[std::to_string(index)]["id"] = getObjectId();
    toReturn[std::to_string(index)]["positionX"] = getPosition().x;
    toReturn[std::to_string(index)]["positionY"] = getPosition().y;
    toReturn[std::to_string(index)]["sizeX"] = getSize().x;
    toReturn[std::to_string(index)]["sizeY"] = getSize().y;
    return toReturn;
}