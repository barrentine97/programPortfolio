#include "Platform.h"

/**
 * @brief Construct a new Platform:: Platform object
 * 
 * @param id the object id
 * @param red the red value
 * @param green the green value
 * @param blue the blue value
 */
Platform::Platform(int id, int red, int green, int blue) {
    setObjectId(id);
    this->red = red;
    this->green = green;
    this->blue = blue;
    setFillColor(sf::Color(red, green, blue));
    setOutlineThickness(2);
    setOutlineColor(sf::Color::Black);
}

/**
 * @brief draws the object to the given window
 * 
 * @param window the window to draw to
 */
void Platform::render(sf::RenderWindow *window) {
    window->draw(*this);
}

/**
 * @brief overrides the abstract class to make this object collidable
 * 
 * @return true since this object is collidable
 */
bool Platform::isCollidable() {
    return true;
}

/**
 * @brief used to horizontally slide the object over
 * 
 * @param offset the distance to slide
 */
void Platform::slideOver(float offset) {
    move(offset, 0);
}

/**
 * @brief returns a json representation of the object
 * 
 * @param index the index of the object in the list of objects
 * @return json the json representatino of the object
 */
json Platform::getJson(int index) {
    json toReturn;
    toReturn[std::to_string(index)]["type"] = "Platform";
    toReturn[std::to_string(index)]["id"] = getObjectId();
    toReturn[std::to_string(index)]["red"] = red;
    toReturn[std::to_string(index)]["blue"] = blue;
    toReturn[std::to_string(index)]["green"] = green;
    toReturn[std::to_string(index)]["positionX"] = getPosition().x;
    toReturn[std::to_string(index)]["positionY"] = getPosition().y;
    toReturn[std::to_string(index)]["sizeX"] = getSize().x;
    toReturn[std::to_string(index)]["sizeY"] = getSize().y;
    return toReturn;
}