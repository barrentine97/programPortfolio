#include <SFML/Graphics.hpp>
#include <thread>

#include "object/ObjectList.h"
#include "character/Character.h"
#include "spawnpoint/SpawnPoint.h"
#include "time/Timeline.h"
#include "lib/zmq.hpp"
#include "lib/json.hpp"

using json = nlohmann::json;

/**
 * @brief main function for a client. Connects to the game server and takes user input to move character through
 *        the servers "world"
 * 
 * @return int system exit status
 */
int main() {
    // create a new window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    bool resizeConstant = true;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket for new players and bind to interface
    zmq::socket_t newPlayerSocket{context, zmq::socket_type::req};
    newPlayerSocket.connect("tcp://localhost:5554");

    // construct a REQ (request) socket for player input and bind to interface
    zmq::socket_t inputSocket{context, zmq::socket_type::req};
    inputSocket.connect("tcp://localhost:5555");

    // construct a PUB (publisher) socket and bind to interface
    zmq::socket_t subSocket{context, zmq::socket_type::sub};
    subSocket.connect("tcp://localhost:5556");
    subSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    // construct a  REQ (request) socket for players disconnecting from server
    zmq::socket_t disconnectSocket{context, zmq::socket_type::req};
    disconnectSocket.connect("tcp://localhost:5557");

    // send initial message to let server know we're connected
    int playerId = -1;
    newPlayerSocket.send(zmq::buffer(std::to_string(playerId)), zmq::send_flags::none);

    // read the reply. It is this player's playerId. Set it for future messages
    zmq::message_t reply;
    newPlayerSocket.recv(reply, zmq::recv_flags::none);
    playerId = stoi(reply.to_string());

    // create list of objects to avoid making new objects every iteration
    std::vector<GameObject *> objects;
    int localPlayers = 0;

    // continue running while the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                disconnectSocket.send(zmq::buffer(std::to_string(playerId)), zmq::send_flags::none);
                window.close();
            }

            // a key has been pressed
            if (event.type == sf::Event::KeyPressed) {
                // if it was 'R', toggle between resizing constantly or proportionally
                if (event.key.code == sf::Keyboard::R) {
                    if (resizeConstant) {
                        resizeConstant = false;
                    } else {
                        resizeConstant = true;
                    }
                }
            }

            // used to keep sizes constant when resizing in this mode
            if (event.type == sf::Event::Resized && resizeConstant) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
    
        // now get input to send to the server
        json player;
        player["id"] = playerId;
        player["p"] = (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && window.hasFocus());
        player["s"] = (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && window.hasFocus());
        player["w"] = (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && window.hasFocus());
        player["left"] = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && window.hasFocus()) ;
        player["right"] = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && window.hasFocus());
        player["up"] = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && window.hasFocus());

        // convert json to string using dump and send to the server
        inputSocket.send(zmq::buffer(player.dump()), zmq::send_flags::none);

        // get a reply. This won't be used
        inputSocket.recv(reply, zmq::recv_flags::none);

        // now receive game state from the server's publish
        subSocket.recv(reply, zmq::recv_flags::none);
        json server = json::parse(reply.to_string());

        // first, remove any disconnected clients if necessary
        int numPlayers = server["numPlayers"].get<int>();
        if (numPlayers < localPlayers) {
            for (int i = 0; i < objects.size(); i++) {
                if (objects[i]->getObjectId() == server["remove"].get<int>()) {
                    objects.erase(objects.begin() + i);
                    localPlayers--;
                    break;
                }
            }
        }

        // update the local list
        for (int i = 0; i < server["numObjects"].get<int>(); i++) {
            bool exists = false;
            for (int j = 0; j < objects.size(); j++) {
                // first, see if it already exists locally and update it
                if (server[std::to_string(i)]["id"].get<int>() == objects[j]->getObjectId()) {
                    exists = true;
                    auto objPtr = dynamic_cast<sf::Shape *>(objects[i]);
                    float x = server[std::to_string(i)]["positionX"].get<float>();
                    float y = server[std::to_string(i)]["positionY"].get<float>();
                    objPtr->setPosition(x, y);
                }
            }

            // in this case, this object doesn't exist yet. Make it and add it to the local list
            if (!exists) {
                std::string objType = server[std::to_string(i)]["type"].get<std::string>();
                int id = server[std::to_string(i)]["id"].get<int>();
                float positionX = server[std::to_string(i)]["positionX"].get<float>();
                float positionY = server[std::to_string(i)]["positionY"].get<float>();

                if ("Character" == objType) {
                    Character *c = new Character(id);
                    sf::Texture *texture = new sf::Texture();
                    // add a texture
                    if (!texture->loadFromFile("textures/sphere.png")) {
                        std::cout << "failure to load texture\n";
                        exit(EXIT_FAILURE);
                    }
                    texture->setSmooth(true);
                    c->setTexture(texture);
                    c->setPosition(positionX, positionY);
                    objects.push_back(c);
                    localPlayers++;

                } else if ("Platform" == objType) {
                    int red = server[std::to_string(i)]["red"].get<int>();
                    int green = server[std::to_string(i)]["green"].get<int>();
                    int blue = server[std::to_string(i)]["blue"].get<int>();
                    float sizeX = server[std::to_string(i)]["sizeX"].get<int>();
                    float sizeY = server[std::to_string(i)]["sizeY"].get<int>();
                    Platform *p = new Platform(id, red, green, blue);
                    p->setPosition(positionX, positionY);
                    p->setSize(sf::Vector2f(sizeX, sizeY));
                    objects.push_back(p);

                } else if ("MovingPlatform" == objType) {
                    int red = server[std::to_string(i)]["red"].get<int>();
                    int green = server[std::to_string(i)]["green"].get<int>();
                    int blue = server[std::to_string(i)]["blue"].get<int>();
                    int direction = server[std::to_string(i)]["direction"].get<int>();
                    int bound1 = server[std::to_string(i)]["bound1"].get<int>();
                    int bound2 = server[std::to_string(i)]["bound2"].get<int>();
                    float sizeX = server[std::to_string(i)]["sizeX"].get<int>();
                    float sizeY = server[std::to_string(i)]["sizeY"].get<int>();
                    MovingPlatform *mp = new MovingPlatform(id, red, green, blue, direction, bound1, bound2);
                    mp->setPosition(positionX, positionY);
                    mp->setSize(sf::Vector2f(sizeX, sizeY));
                    objects.push_back(mp);

                } else if ("DeathZone" == objType) {
                    float sizeX = server[std::to_string(i)]["sizeX"].get<int>();
                    float sizeY = server[std::to_string(i)]["sizeY"].get<int>();
                    DeathZone *dz = new DeathZone(id);
                    dz->setPosition(positionX, positionY);
                    dz->setSize(sf::Vector2f(sizeX, sizeY));
                    objects.push_back(dz);

                } else if ("SideBoundary" == objType) {
                    float sizeX = server[std::to_string(i)]["sizeX"].get<int>();
                    float sizeY = server[std::to_string(i)]["sizeY"].get<int>();
                    SideBoundary *sb = new SideBoundary(id);
                    sb->setPosition(positionX, positionY);
                    sb->setSize(sf::Vector2f(sizeX, sizeY));
                    objects.push_back(sb);

                } else if ("SpawnPoint" == objType) {
                    SpawnPoint *sp = new SpawnPoint(id, positionX, positionY);
                    sp->setPosition(positionX, positionY);
                    objects.push_back(sp);

                }
            }
        }

        // now that everything has been updated, render the frame
        window.clear(sf::Color::White);
        for (int i = 0; i < objects.size(); i++) {
            objects[i]->render(&window);
        }

        window.display();
    }

    return 0;
}