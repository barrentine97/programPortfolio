#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <condition_variable>
#include <vector>

#include "../lib/json.hpp"
using json = nlohmann::json;

class GameObject {
    private:
        int objectId;

    public:
        int getObjectId();
        void setObjectId(int id);
        virtual void render(sf::RenderWindow *window) {};
        virtual void moveObject(std::vector<GameObject *> objects, int deltaTime) {};
        virtual bool isCollidable() { return false; };
        virtual void slideOver(float offset) {};
        virtual json getJson(int index) = 0;
};