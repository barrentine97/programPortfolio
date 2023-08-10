#pragma once
#include <SFML/Graphics.hpp>

#include "../object/GameObject.h"

#include "../lib/json.hpp"
using json = nlohmann::json;

class SideBoundary : public sf::RectangleShape, public GameObject {
    public:
        SideBoundary(int id);
        virtual bool isCollidable();
        virtual json getJson(int index);
};