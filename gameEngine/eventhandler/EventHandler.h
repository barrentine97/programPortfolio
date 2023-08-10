#pragma once
#include <iostream>
#include "../event/Event.h"

class EventHandler {
    public:
        virtual void onEvent(Event *event) = 0;
        //virtual void onEvent(Event *e) { std::cout << "oops wrong spot\n"; };
};