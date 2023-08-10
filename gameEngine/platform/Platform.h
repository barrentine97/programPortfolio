#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../object/GameObject.h"

#include "../lib/json.hpp"
using json = nlohmann::json;

class Platform : public sf::RectangleShape, public GameObject {
    private:
        int red;
        int green;
        int blue;
    
    public:
        Platform(int id, int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();
        virtual void render(sf::RenderWindow *window);
        virtual bool isCollidable();
        virtual void slideOver(float offset);
        virtual json getJson(int index);
};