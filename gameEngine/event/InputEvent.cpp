#include "InputEvent.h"

/**
 * @brief Construct a new Input Event:: Input Event object
 * 
 * @param character the character that the input is for
 * @param left bool if left is pressed
 * @param right bool if right is pressed
 * @param up bool if up is pressed
 */
InputEvent::InputEvent(Character *character, bool left, bool right, bool up) {
    this->type = TYPE_INPUT;
    this->character = character;
    this->left = left;
    this->right = right;
    this->up = up;
}