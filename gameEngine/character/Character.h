#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <SFML/Graphics.hpp>

#include "../deathzone/DeathZone.h"
#include "../eventmanager/EventManager.h"
#include "../event/CollisionEvent.h"
#include "../object/GameObject.h"
#include "../object/ObjectList.h"
#include "../sideboundary/SideBoundary.h"
#include "../platform/Platform.h"
#include "../platform/MovingPlatform.h"
#include "../time/Timeline.h"

#include "../lib/json.hpp"
using json = nlohmann::json;

class Character : public sf::CircleShape, public GameObject {
    private:
        bool onGround;
        float xVelocity;
        float yVelocity;
        float gravity = 0.04f;
        bool dead;
        bool left;
        bool right;
        bool up;
        std::mutex *_mutex;
        std::condition_variable *_condition_variable;
    
    public:
        Character(int id);
        void setMutex(std::mutex *_mutex);
        void setConditionVariable(std::condition_variable *_condition_variable);
        bool isDead();
        void setDead(bool dead);
        void setLeft(bool left);
        void setRight(bool right);
        void setUp(bool up);
        float getVelocityX();
        void setVelocityY(float y);
        void setOnGround(bool onGround);
        virtual void render(sf::RenderWindow *window);
        virtual void moveObject(std::vector<GameObject *> objects, int deltaTime);
        virtual void slideOver(float offset);
        virtual json getJson(int index);
};