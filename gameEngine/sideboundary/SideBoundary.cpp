#include "SideBoundary.h"

/**
 * @brief Construct a new Side Boundary:: Side Boundary object
 * 
 * @param id the object id
 */
SideBoundary::SideBoundary(int id) {
    setObjectId(id);
}

/**
 * @brief overrides the abstract class to make this object collidable
 * 
 * @return true since the object is collidable
 */
bool SideBoundary::isCollidable() {
    return true;
}

/**
 * @brief returns a json respresentation of the object
 * 
 * @param index the index of the object in the list of objects
 * @return json the json representation of the object
 */
json SideBoundary::getJson(int index) {
    json toReturn;
    toReturn[std::to_string(index)]["type"] = "SideBoundary";
    toReturn[std::to_string(index)]["id"] = getObjectId();
    toReturn[std::to_string(index)]["positionX"] = getPosition().x;
    toReturn[std::to_string(index)]["positionY"] = getPosition().y;
    toReturn[std::to_string(index)]["sizeX"] = getSize().x;
    toReturn[std::to_string(index)]["sizeY"] = getSize().y;
    return toReturn;
}