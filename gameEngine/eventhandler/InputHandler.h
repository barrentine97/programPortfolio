#pragma once
#include "EventHandler.h"
#include "../event/InputEvent.h"

class InputHandler : public EventHandler {
    public:
        void onEvent(Event *event);
};