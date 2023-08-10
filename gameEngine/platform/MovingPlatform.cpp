#include "MovingPlatform.h"

/**
 * @brief Construct a new Moving Platform:: Moving Platform object
 * 
 * @param id the object id
 * @param red the red value
 * @param green the green value
 * @param blue the blue value
 * @param direction the direction of the platform
 * @param bound1 the first bound for the platform to move between
 * @param bound2 the second bound for the platform to move between
 */
MovingPlatform::MovingPlatform(int id, int red, int green, int blue, int direction, int bound1, int bound2) {
    setObjectId(id);
    this->red = red;
    this->green = green;
    this->blue = blue;
    setFillColor(sf::Color(red, green, blue));
    setOutlineThickness(2);
    setOutlineColor(sf::Color::Black);
    this->direction = direction;
    this->bound1 = bound1;
    this->bound2 = bound2;
    this->velocity = VELOCITY;
}

/**
 * @brief returns the platforms direction
 *        1 = north, 2 = east, 3 = south, 4 = west
 * 
 * @return int the platforms direction
 */
int MovingPlatform::getDirection() {
    return direction;
}

/**
 * @brief returns the platforms velocity
 * 
 * @return float the platforms velocity
 */
float MovingPlatform::getVelocity() {
    return velocity;
}

/**
 * @brief draws the object to the given window
 * 
 * @param window the window to draw to
 */
void MovingPlatform::render(sf::RenderWindow *window) {
    window->draw(*this);
}

/**
 * @brief overrides the abstract class to make this object collidable
 * 
 * @return true since this object is collidable
 */
bool MovingPlatform::isCollidable() {
    return true;
}

/**
 * @brief used to horizontally slide the object over
 * 
 * @param offset the distance to slide
 */
void MovingPlatform::slideOver(float offset) {
    move(offset, 0);
    if (direction == 2 || direction == 4) {
        this->bound1 += offset;
        this->bound2 += offset;
    }
}

/**
 * @brief moves the platform using the direction, velocity, and bounds of the platform
 * 
 * @param objects the objects
 * @param deltaTime the deltaTime used to calculate velocity
 */
void MovingPlatform::moveObject(std::vector<GameObject *> objects, int deltaTime) {
    if (direction == 1) {
        move(0, -1 * velocity * deltaTime);
        if (getPosition().y < bound1) {
            this->direction = 3;
        }
    } else if (direction == 2) {
        move(velocity * deltaTime, 0);
        if (getPosition().x > bound2) {
            this->direction = 4;
        }
    } else if (direction == 3) {
        move(0, velocity * deltaTime);
        if (getPosition().y > bound2) {
            this->direction = 1;
        }
    } else if (direction == 4) {
        move(-1 * velocity * deltaTime, 0);
        if (getPosition().x < bound1) {
            this->direction = 2;
        }
    }
}

/**
 * @brief returns a json representation of the object
 * 
 * @param index the index of the object in the list of objects
 * @return json the json representation of the object
 */
json MovingPlatform::getJson(int index) {
    json toReturn;
    toReturn[std::to_string(index)]["type"] = "MovingPlatform";
    toReturn[std::to_string(index)]["id"] =getObjectId();
    toReturn[std::to_string(index)]["red"] = red;
    toReturn[std::to_string(index)]["blue"] = blue;
    toReturn[std::to_string(index)]["green"] = green;
    toReturn[std::to_string(index)]["direction"] = direction;
    toReturn[std::to_string(index)]["bound1"] = bound1;
    toReturn[std::to_string(index)]["bound2"] = bound2;
    toReturn[std::to_string(index)]["positionX"] = getPosition().x;
    toReturn[std::to_string(index)]["positionY"] = getPosition().y;
    toReturn[std::to_string(index)]["sizeX"] = getSize().x;
    toReturn[std::to_string(index)]["sizeY"] = getSize().y;
    return toReturn;
}