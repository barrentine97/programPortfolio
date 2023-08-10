#include "DeathEvent.h"

/**
 * @brief Construct a new Death Event:: Death Event object
 * 
 * @param character the character that dies
 */
DeathEvent::DeathEvent(Character *character) {
    this->type = TYPE_DEATH;
    this->character = character;
}