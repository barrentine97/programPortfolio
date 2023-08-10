#include "GameObject.h"

/**
 * @brief returns the objects unique id
 * 
 * @return int the objects unique id
 */
int GameObject::getObjectId() {
    return objectId;
}

/**
 * @brief sets the objects unique id to the given parameter
 * 
 * @param id the id to set
 */
void GameObject::setObjectId(int id) {
    this->objectId = id;
}