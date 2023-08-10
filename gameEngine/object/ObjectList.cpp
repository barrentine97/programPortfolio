#include "ObjectList.h"

/**
 * @brief Construct a new Object List:: Object List object
 * 
 * @param _mutex the mutex used for multithreading
 * @param _condition_variable the condition_variable used for multithreading
 */
ObjectList::ObjectList(std::mutex *_mutex, std::condition_variable *_condition_variable) {
    this->numObjects = 0;
    this->numPlayers = 0;
    this->nextId = 0;
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
    this->busy = true;
}

/**
 * @brief returns the busy state of the list
 * 
 * @return true if the list is busy
 * @return false if the list is not
 */
bool ObjectList::isBusy() {
    return busy;
}

/**
 * @brief sets the busy state to the given parameter
 * 
 * @param val the value to set
 */
void ObjectList::setBusy(bool val) {
    std::unique_lock<std::mutex> cv_lock(*this->_mutex);
    this->busy = val;
}

/**
 * @brief returns the list of objects
 * 
 * @return std::vector<GameObject *> the list of objects
 */
std::vector<GameObject *> ObjectList::getList() {
    return objects;
}

/**
 * @brief adds the object to the list
 * 
 * @param o the object to add
 */
void ObjectList::addToList(GameObject *o) {
    this->objects.push_back(o);
    numObjects++;
}

/**
 * @brief adds the player to the list. This is the same as addtoList() except the numPlayers variable is incremented
 * 
 * @param o the object to add
 */
void ObjectList::addPlayerToList(GameObject *o) {
    addToList(o);
    numPlayers++;
}

/**
 * @brief returns the number of objects in the list
 * 
 * @return int the number of objects in the list
 */
int ObjectList::getNumObjects() {
    return numObjects;
}

/**
 * @brief returns the number of players in the list
 * 
 * @return int the number of players in the list
 */
int ObjectList::getNumPlayers() {
    return numPlayers;
}

/**
 * @brief returns the next id to be given to a new object
 * 
 * @return int the next id to be given to a new object
 */
int ObjectList::getNextId() {
    return nextId++;
}

/**
 * @brief removes the object with the given id from the list
 * 
 * @param id the id of the object to remove
 */
void ObjectList::removeFromList(int id) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->getObjectId() == id) {
            objects.erase(objects.begin() + i);
            break;
        }
    }
    
    numPlayers--;
    numObjects--;
}

/**
 * @brief returns the object with the given id from the list
 * 
 * @param id the id of the object to find
 * @return GameObject* pointer to the object with the given id
 */
GameObject* ObjectList::getObject(int id) {
    GameObject *o;
    for (int i = 0; i < objects.size(); i++) {
        o = objects[i];
        if (o->getObjectId() == id) {
            break;
        }
    }

    return o;
}

/**
 * @brief move all the character objects in the list
 * 
 * @param deltaTime the deltaTime used to calculate velocity
 */
void ObjectList::moveCharacters(int deltaTime) {
    for (int i = 0; i < objects.size(); i++) {
        // first acquire lock and check if platforms are finished
        std::unique_lock<std::mutex> lock(*this->_mutex);
        while (busy) {
            this->_condition_variable->wait(lock);    
        }
        
        // now that the platforms are finished, move the players
        auto myPtr = dynamic_cast<Character *>(objects[i]);
        if (myPtr != nullptr) {
            objects[i]->moveObject(objects, deltaTime);
        }
    }
}

/**
 * @brief move all the platform objects in the list
 * 
 * @param deltaTime the deltaTime used to calculate the velocity
 */
void ObjectList::movePlatforms(int deltaTime) {
    for (int i = 0; i < objects.size(); i++) {
        // first, acquire the lock
        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
        // not check if its a character
        auto myPtr = dynamic_cast<Character *>(objects[i]);
        // if not, move it
        if (myPtr == nullptr) {
            objects[i]->moveObject(objects, deltaTime);
        }
    }

    setBusy(false);
    this->_condition_variable->notify_one();
}