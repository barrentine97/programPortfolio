#pragma once
#include <SFML/Graphics.hpp>

#include "../object/GameObject.h"

#include "../lib/json.hpp"
using json = nlohmann::json;

class DeathZone : public sf::RectangleShape, public GameObject {
    public:
        DeathZone(int id);
        virtual bool isCollidable();
        virtual json getJson(int index);
};