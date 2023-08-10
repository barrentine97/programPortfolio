#include <thread>
#include <SFML/Graphics.hpp>

#include "character/Character.h"
#include "deathzone/DeathZone.h"
#include "event/InputEvent.h"
#include "eventhandler/CollisionHandler.h"
#include "eventhandler/DeathHandler.h"
#include "eventhandler/SpawnHandler.h"
#include "eventhandler/InputHandler.h"
#include "eventmanager/EventManager.h"
#include "object/ObjectList.h"
#include "platform/Platform.h"
#include "platform/MovingPlatform.h"
#include "sideboundary/SideBoundary.h"
#include "spawnpoint/SpawnPoint.h"
#include "time/Timeline.h"

#include "lib/zmq.hpp"
#include "lib/json.hpp"

using json = nlohmann::json;

/**
 * @brief wrapper function for thread to run inside Character class
 * 
 * @param p player for thread to use
 * @param list list of platforms for player to use
 */
void playerWrapper(ObjectList *ObjectList, int *deltaTime) {
    ObjectList->moveCharacters(*deltaTime);
}

/**
 * @brief wrapper function for thread to run inside ObjectList class
 * 
 * @param list Objectlist for thread to use
 */
void platformWrapper(ObjectList *objectList, int *deltaTime) {
    objectList->movePlatforms(*deltaTime);
}

/**
 * @brief main function. Handles start-up, main-loop, and (eventually) shutdown of game engine server
 * 
 * @return int system exit status 
 */
int main() {

    // create a new window for debugging
    // sf::RenderWindow testWindow(sf::VideoMode(800, 600), "My window", sf::Style::Default);
    // testWindow.setVerticalSyncEnabled(true);
    // testWindow.setKeyRepeatEnabled(false);
    // bool resizeConstant = true;

    // create mutex and condition variable for multi-threading
    std::mutex m;
    std::condition_variable cv;

    // create timeline object for real time
    Timeline *realTime = new Timeline(NULL, 1);

    // create timeline object for game time, anchored to real time
    Timeline *gameTime = new Timeline(realTime, 1);
    int lastTime = gameTime->getTime();

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket for new players and bind to interface
    zmq::socket_t newPlayerSocket{context, zmq::socket_type::rep};
    newPlayerSocket.bind("tcp://*:5554");

    // construct a REP (reply) socket for player input and bind to interface
    zmq::socket_t inputSocket{context, zmq::socket_type::rep};
    inputSocket.bind("tcp://*:5555");

    // construct a PUB (publisher) socket and bind to interface
    zmq::socket_t pubSocket{context, zmq::socket_type::pub};
    pubSocket.bind("tcp://*:5556");

    // construct a REP (reply) socket for player input and bind to interface
    zmq::socket_t disconnectSocket{context, zmq::socket_type::rep};
    disconnectSocket.bind("tcp://*:5557");

    // create the ObjectList object to keep track of all objects
    ObjectList *objectList = new ObjectList(&m, &cv);

    // create a stationary platform
    Platform *platform1 = new Platform(objectList->getNextId(), 150, 150, 150);
    platform1->setSize(sf::Vector2f(300.f, 50.f));
    platform1->setPosition(50, 400);
    objectList->addToList(platform1);

    // create a moving platform
    MovingPlatform *movingPlatform1 = new MovingPlatform(objectList->getNextId(), 180, 0, 0, 1, 200, 400);
    movingPlatform1->setSize(sf::Vector2f(300.f, 50.f));
    movingPlatform1->setPosition(400, 400);
    objectList->addToList(movingPlatform1);

    // create second moving platform
    MovingPlatform *movingPlatform2 = new MovingPlatform(objectList->getNextId(), 150, 0, 200, 4, 750, 950);
    movingPlatform2->setSize(sf::Vector2f(300.f, 50.f));
    movingPlatform2->setPosition(750, 200);
    objectList->addToList(movingPlatform2);

    // create a third moving platform
    MovingPlatform *movingPlatform3 = new MovingPlatform(objectList->getNextId(), 0, 125, 30, 1, 200, 400);
    movingPlatform3->setSize(sf::Vector2f(300.f, 50.f));
    movingPlatform3->setPosition(1300, 200);
    objectList->addToList(movingPlatform3);

    // create a second stationary platform
    Platform *platform2 = new Platform(objectList->getNextId(), 0, 125, 200);
    platform2->setSize(sf::Vector2f(300.f, 50.f));
    platform2->setPosition(1700, 400);
    objectList->addToList(platform2);

    // create death zone
    DeathZone *deathZone = new DeathZone(objectList->getNextId());
    deathZone->setSize(sf::Vector2f(800.f, 50.f));
    deathZone->setPosition(0, 500);
    objectList->addToList(deathZone);

    // create side boundaries
    SideBoundary *leftBoundary = new SideBoundary(objectList->getNextId());
    leftBoundary->setSize(sf::Vector2f(50.f, 600.f));
    leftBoundary->setPosition(50, -100);
    objectList->addToList(leftBoundary);

    SideBoundary *rightBoundary = new SideBoundary(objectList->getNextId());
    rightBoundary->setSize(sf::Vector2f(50.f, 600.f));
    rightBoundary->setPosition(600, -100);
    objectList->addToList(rightBoundary);

    // create the spawn point for the characters
    SpawnPoint *spawnPoint = new SpawnPoint(objectList->getNextId(), 150, 100);
    objectList->addToList(spawnPoint);

    // create all event handlers
    CollisionHandler *ch = new CollisionHandler();
    DeathHandler *dh = new DeathHandler();
    SpawnHandler *sh = new SpawnHandler(spawnPoint);
    InputHandler *ih = new InputHandler();

    // set up event manager, giving it a timeline and registering all handlers
    EventManager *em = EventManager::getInstance();
    em->setTimeline(gameTime);
    em->registerHandler(Event::TYPE_COLLISION, ch);
    em->registerHandler(Event::TYPE_DEATH, dh);
    em->registerHandler(Event::TYPE_SPAWN, sh);
    em->registerHandler(Event::TYPE_INPUT, ih);

    // run the program as long as the server is running
    while (true) {

        // this is the json that will be published for clients
        json pub;

        // compute the deltaTime for this frame
        int currentTime = gameTime->getTime();
        int deltaTime = currentTime - lastTime;

        // adjust based on tic size
        deltaTime /= gameTime->getTic();

        // update current time and last time
        currentTime = gameTime->getTime();
        lastTime = currentTime;

        // now check for any new clients
        zmq::message_t request;
        zmq::recv_result_t ret = newPlayerSocket.recv(request, zmq::recv_flags::dontwait);
        // this indicates a new player is waiting to join
        if (ret.has_value()) {
            int id = objectList->getNextId();

            // reply to the player with it's player ID
            newPlayerSocket.send(zmq::buffer(std::to_string(id)), zmq::send_flags::none);

            // create a new player with the next available ID
            Character *player = new Character(id);
            player->setFillColor(sf::Color::Blue);

            // add it to the list and increment appropriate counters
            objectList->addPlayerToList(player);
        }

        ret = disconnectSocket.recv(request, zmq::recv_flags::dontwait);
        // this indicates a player is trying to disconnect
        if (ret.has_value()) {
            // get the id for the object
            int id = stoi(request.to_string());

            // remove it from the list
            objectList->removeFromList(id);

            // and let the client know
            std::string reply {"Successfully disconnected"};
            disconnectSocket.send(zmq::buffer(reply), zmq::send_flags::none);
            pub["remove"] = id;
        }

        // update all player objects with message. This includes spawning any dead players
        // or any players that just joined at the spawn point.
        for (int i = 0; i < objectList->getNumPlayers(); i++) {
            // first, get input for the player and save it to that player
            inputSocket.recv(request, zmq::recv_flags::none);
            json input = json::parse(request.to_string());

            // if the player pressed 'P', pause/unpause the game
            if (input["p"].get<bool>()) {
                if (gameTime->isPaused()) {
                    lastTime = gameTime->getTime();
                    gameTime->unpause();
                } else {
                    gameTime->pause();
                }
            }

            // if the player pressed 'S', speed the game up
            if (input["s"].get<bool>()) {
                float tic = gameTime->getTic();
                if (tic == 2) {
                    gameTime->changeTic(1);
                } else if (tic == 1) {
                    gameTime->changeTic(0.5);
                }
            }

            // if the player pressed 'W', slow the game down
            if (input["w"].get<bool>()) {
                float tic = gameTime->getTic();
                if (tic == 0.5) {
                    gameTime->changeTic(1);
                } else if (tic == 1) {
                    gameTime->changeTic(2);
                }
            }

            int id = input["id"].get<int>();
            Character *c = dynamic_cast<Character *>(objectList->getObject(id));
            if (c->isDead()) {
                spawnPoint->spawn(c);
            }

            InputEvent *ie = new InputEvent(c, input["left"].get<bool>(),
                                               input["right"].get<bool>(),
                                               input["up"].get<bool>());
            em->raise(ie);

            std::string reply {"Updating your player position"};
            inputSocket.send(zmq::buffer(reply), zmq::send_flags::none);
        }

        // update player locations with new thread
        std::thread playerThread(playerWrapper, objectList, &deltaTime);

        // update platform locations with a new thread
        std::thread platformThread(platformWrapper, objectList, &deltaTime);
        
        playerThread.join();
        platformThread.join();

        EventManager::handleEvents();

        // now publish all positions for clients
        std::vector<GameObject *> objects = objectList->getList();
        pub["numObjects"] = objectList->getNumObjects();
        pub["numPlayers"] = objectList->getNumPlayers();
        for (int i = 0; i < objectList->getNumObjects(); i++) {
            pub.update(objects[i]->getJson(i));
        }

        // publish for players
        pubSocket.send(zmq::buffer(pub.dump()), zmq::send_flags::none);

        // print all objects for debugging
        // testWindow.clear(sf::Color::White);
        // for (int i = 0; i < objects.size(); i++) {
        //     {
        //         auto ptr = dynamic_cast<DeathZone *>(objects[i]);
        //         if (ptr != nullptr) {
        //             ptr->setFillColor(sf::Color::Black);
        //             testWindow.draw(*ptr);
        //         }
        //     }
        //     {
        //         auto ptr = dynamic_cast<SideBoundary *>(objects[i]);
        //         if (ptr != nullptr) {
        //             ptr->setFillColor(sf::Color::Cyan);
        //             testWindow.draw(*ptr);
        //         }

        //     }
        //     {
        //         auto ptr = dynamic_cast<SpawnPoint *>(objects[i]);
        //         if (ptr != nullptr) {
        //             ptr->setFillColor(sf::Color::Green);
        //             testWindow.draw(*ptr);
        //         }
        //     }
        //     objects[i]->render(&testWindow);
        // }
        // testWindow.display();
    }

    return 0;
};