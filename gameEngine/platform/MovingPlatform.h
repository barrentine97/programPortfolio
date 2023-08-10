#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../object/GameObject.h"

#include "../lib/json.hpp"
using json = nlohmann::json;

#define VELOCITY 0.02

class MovingPlatform : public sf::RectangleShape, public GameObject {
    private:
        int red;
        int green;
        int blue;
        int direction;
        float bound1;
        float bound2;
        float velocity;
    
    public:
        MovingPlatform(int id, int red, int green, int blue, int direction, int bound1, int bound2);
        int getDirection();
        float getVelocity();
        virtual void render(sf::RenderWindow *window);
        virtual bool isCollidable();
        virtual void slideOver(float offset);
        virtual void moveObject(std::vector<GameObject *> objects, int deltaTime);
        virtual json getJson(int index);
};