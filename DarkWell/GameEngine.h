#pragma once

#include "Player.h"
#include "Room.h"
#include "Obstacle.h"

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    int StartGame();
    List<Room*> rooms;

private:
    void drawInventoryOverlay(sf::RenderWindow& window, Player& player, sf::RectangleShape& lazerGunShape, sf::RectangleShape& shovelShape);
    Room* initializeRoom1();  // Method to initialize Room 1
    Room* initializeRoom2();  // Method to initialize Room 2
    Room* initializeRoom3();  // Method to initialize Room 3
    Room* initializeRoom4();  // Method to initialize Room 4
    void handleEvents(sf::RenderWindow& window);  // Method to handle window events
    void updateObstacles(Room* currentRoom, float deltaTime);  // Method to update obstacles
    void handleRoomTransitions(Player& player, Room*& currentRoom, List<Room*>& rooms);  // Method for room transitions
    void cleanUp();  // Method to clean up rooms
};
