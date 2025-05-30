#pragma once

#include "Player.h"
#include "Room.h"
#include "Obstacle.h"
#include <SFML/Audio.hpp>

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    int StartGame();
    
private:
    std::string currentMusic;
    sf::Music backgroundMusic;
    sf::Music eventSoundEffect;
    List<Room*> rooms;
    bool gamePaused;
    void drawInventoryOverlay(
        sf::RenderWindow& window, 
        Player& player, 
        sf::RectangleShape& lazerGunShape, 
        sf::RectangleShape& shovelShape);
    Room* initializeRoom1();  // Method to initialize Room 1
    Room* initializeRoom2();  // Method to initialize Room 2
    Room* initializeRoom3();  // Method to initialize Room 3
    Room* initializeRoom4();  // Method to initialize Room 4
    Room* initializeRoom5();  // Method to initialize Room 5
    Room* initializeRoom6();  // Method to initialize Room 6
    Room* initializeRoom7();  // Method to initialize Room 7
    Room* initializeRoom8();  // Method to initialize Room 8
    void handleEvents(sf::RenderWindow& window);  // Method to handle window events
    void updateObstacles(Room* currentRoom, float deltaTime);  // Method to update obstacles
    void handleRoomTransitions(Player& player, Room*& currentRoom, List<Room*>& rooms, bool& isRespawning);  // Method for room transitions
    void cleanUp();  // Method to clean up rooms
    void spawnNPC(Room* currentRoom, float deltaTime);
    void respawnNPC(); // Method to respawn all NPC
    void respawnNPCdead(); // Method to respawn all NPC when player dies
    void win(Player player, sf::RenderWindow& window);
};
