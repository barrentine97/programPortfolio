#pragma once
#include "GameObject.h"
#include "../character/Character.h"

class ObjectList {
    private:
        int numObjects;
        int numPlayers;
        int nextId;
        std::vector<GameObject *> objects;
        bool busy;
        std::mutex *_mutex;
        std::condition_variable *_condition_variable;
    
    public:
        ObjectList(std::mutex *_mutex, std::condition_variable *_condition_variable);
        bool isBusy();
        void setBusy(bool val);
        std::vector<GameObject *> getList();
        void addToList(GameObject *o);
        void addPlayerToList(GameObject *o);
        void removeFromList(int id);
        GameObject* getObject(int id);
        int getNumObjects();
        int getNumPlayers();
        int getNextId();
        void moveCharacters(int deltaTime);
        void movePlatforms(int deltaTime);
};