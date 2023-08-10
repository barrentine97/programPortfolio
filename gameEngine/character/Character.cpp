#include "Character.h"

/**
 * @brief Construct a new Character:: Character object
 * 
 * @param id the object id
 */
Character::Character(int id) {
    setObjectId(id);
    this->xVelocity = 0.25;
    setVelocityY(0);
    setRadius(35.f);
    setDead(true);
}

/**
 * @brief sets the mutex to the given mutex
 * 
 * @param _mutex the mutex to set
 */
void Character::setMutex(std::mutex *_mutex) {
    this->_mutex = _mutex;
}

/**
 * @brief sets the condition variable to the given condition variable
 * 
 * @param _condition_variable the condition variable to set
 */
void Character::setConditionVariable(std::condition_variable *_condition_variable) {
    this->_condition_variable = _condition_variable;
}

/**
 * @brief returns the dead status of the character
 * 
 * @return true if character is dead
 * @return false if not
 */
bool Character::isDead() {
    return dead;
}

/**
 * @brief sets the dead status to the given parameter
 * 
 * @param dead the value to set
 */
void Character::setDead(bool dead) {
    this->dead = dead;
}

/**
 * @brief sets the left value to the given parameter
 * 
 * @param left the value to set
 */
void Character::setLeft(bool left) {
    this->left = left;
}

/**
 * @brief sets the right value to the given parameter
 * 
 * @param right the value to set
 */
void Character::setRight(bool right) {
    this->right = right;
}

/**
 * @brief sets the up value to the given parameter
 * 
 * @param up the value to set
 */
void Character::setUp(bool up) {
    this->up = up;
}

float Character::getVelocityX() {
    return xVelocity;
}

/**
 * @brief sets the y velocity for the character
 * 
 * @param y the value to set
 */
void Character::setVelocityY(float y) {
    this->yVelocity = y;
}

/**
 * @brief sets ground boolean to the given value
 * 
 * @param onGround the value to set
 * @return true if the character is on the ground
 * @return false is the character is not
 */
void Character::setOnGround(bool onGround) {
    this->onGround = onGround;
}

/**
 * @brief draws the character to the given window
 * 
 * @param window the window to draw to
 */
void Character::render(sf::RenderWindow *window) {
    window->draw(*this);
}

/**
 * @brief moves the character based on the buttons pressed
 * 
 * @param objects the list of game objects to consider for collision
 * @param deltaTime the deltaTime to calculate the character's velocity
 */
void Character::moveObject(std::vector<GameObject *> objects, int deltaTime) {

    EventManager *em = EventManager::getInstance();

    // move character left if left key is pressed
    if (left) {
        float offset = -1 * xVelocity * deltaTime;
        move(offset, 0);
        // and check for collision
        sf::FloatRect charBox = getGlobalBounds();

        // iterate through all objects and only focus on collidable objects
        for (int i = 0; i < objects.size(); i++) {
            if (getObjectId() == objects[i]->getObjectId()) {
                break;
            }

            if (objects[i]->isCollidable()) {
                // first, cast the object
                auto shapePtr = dynamic_cast<sf::Shape *>(objects[i]);
                sf::FloatRect platBox = shapePtr->getGlobalBounds();

                // and determine if it intersects
                if (charBox.intersects(platBox)) {
                    Event *e = new CollisionEvent(this, objects[i], objects, deltaTime, 1);
                    em->raise(e);
                }
            }
        }
    }

    // move character right if right key is pressed
    if (right) {
        float offset = 1 * xVelocity * deltaTime;
        move(offset, 0);
        // and check for collision
        sf::FloatRect charBox = getGlobalBounds();

        // iterate through all objects and only focus on collidable objects
        for (int i = 0; i < objects.size(); i++) {
            if (getObjectId() == objects[i]->getObjectId()) {
                break;
            }

            if (objects[i]->isCollidable()) {
                // first, cast the object
                auto shapePtr = dynamic_cast<sf::Shape *>(objects[i]);
                sf::FloatRect platBox = shapePtr->getGlobalBounds();

                // and determine if it intersects
                if (charBox.intersects(platBox)) {
                    Event *e = new CollisionEvent(this, objects[i], objects, deltaTime, -1);
                    em->raise(e);
                }
            }
        }
    }

    // add velocity to character if up key is pressed
    // and the character is already on the ground
    if (up && onGround) {
        yVelocity = -0.5f * deltaTime;
        onGround = false;
    }

    // finally, move down from gravity
    if (deltaTime != 0) {
        yVelocity += gravity * deltaTime;
        move(0, yVelocity);
    }
    
    // and check for collision
    sf::FloatRect charBox = getGlobalBounds();

    // iterate through all objects and only focus on collidable objects
    for (int i = 0; i < objects.size(); i++) {
        if (getObjectId() == objects[i]->getObjectId()) {
            break;
        }

        if (objects[i]->isCollidable()) {
            // first, cast the object
            auto shapePtr = dynamic_cast<sf::Shape *>(objects[i]);
            sf::FloatRect platBox = shapePtr->getGlobalBounds();

            // and determine if it intersects
            if (charBox.intersects(platBox)) {
                Event *e = new CollisionEvent(this, objects[i], objects, deltaTime);
                em->raise(e);
            }
        }
    }
}

/**
 * @brief used to horizontally slide the object over
 * 
 * @param offset the distance to slide
 */
void Character::slideOver(float offset) {
    move(offset, 0);
}

/**
 * @brief returns a json representation of the object
 * 
 * @param index the index of the object in the list of objects
 * @return json the json representation of the object
 */
json Character::getJson(int index) {
    json toReturn;
    toReturn[std::to_string(index)]["type"] = "Character";
    toReturn[std::to_string(index)]["id"] = getObjectId();
    toReturn[std::to_string(index)]["positionX"] = getPosition().x;
    toReturn[std::to_string(index)]["positionY"] = getPosition().y;
    return toReturn;
}