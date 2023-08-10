#include "SpawnEvent.h"

/**
 * @brief Construct a new Spawn Event:: Spawn Event object
 * 
 * @param character the character that spawns
 */
SpawnEvent::SpawnEvent(Character *character) {
    this->type = TYPE_SPAWN;
    this->spawnPoint = spawnPoint;
    this->character = character;
}