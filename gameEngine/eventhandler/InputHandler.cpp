#include "InputHandler.h"

/**
 * @brief function that handles input event
 * 
 * @param event the input event to handle
 */
void InputHandler::onEvent(Event *event) {
    auto *e = static_cast<InputEvent *>(event);
    e->character->setLeft(e->left);
    e->character->setRight(e->right);
    e->character->setUp(e->up);
}