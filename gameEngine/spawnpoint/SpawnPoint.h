#pragma once
#include <SFML/Graphics.hpp>

#include "../character/Character.h"

#include "../lib/json.hpp"
using json = nlohmann::json;

class SpawnPoint : public sf::CircleShape, public GameObject {
    private:
        int positionX;
        int positionY;
    
    public:
        SpawnPoint(int id, int x, int y);
        void spawn(Character *c);
        virtual json getJson(int index);
};