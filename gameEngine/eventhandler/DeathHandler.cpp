#include "DeathHandler.h"

/**
 * @brief function that handles a death event
 * 
 * @param event the death event to handle
 */
void DeathHandler::onEvent(Event *event) {
    auto *e = static_cast<DeathEvent *>(event);
    e->character->setDead(true);
}