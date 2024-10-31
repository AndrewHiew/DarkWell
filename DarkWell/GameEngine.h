#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Room.h"
#include "MovingObstacle.h"
#include "GameMap.h"  // Include updated header for GameMap

class GameEngine {
private:
    GameMap* gameMap;  // Updated to GameMap pointer for the room structure

public:
    GameEngine();
    ~GameEngine();

    int StartGame();

    Room* initializeRoom1();
    Room* initializeRoom2();
    Room* initializeRoom3();

    void handleEvents(sf::RenderWindow& window);
    void updateObstacles(Room* currentRoom, float deltaTime);
    void handleRoomTransitions(Player& player);
    void cleanUp();
};
