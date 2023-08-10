#include "SpawnHandler.h"

/**
 * @brief Construct a new Spawn Handler:: Spawn Handler object
 * 
 * @param spawnPoint the spawnpoint to use when handling a spawn event
 */
SpawnHandler::SpawnHandler(SpawnPoint *spawnPoint) {
    this->spawnPoint = spawnPoint;
}

/**
 * @brief function that handles a spawn event
 * 
 * @param event the spawn event to handle
 */
void SpawnHandler::onEvent(Event *event) {
    auto *e = static_cast<SpawnEvent *>(event);
    e->spawnPoint->spawn(e->character);
} 