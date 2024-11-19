#pragma once
#include "ItemObstacle.h"
#include "NormalObstacle.h"
#include "MovingObstacle.h"
#include "KillObstacle.h"
#include "List.h"
#include "Player.h"
#include "Undead.h"
#include "Juggernaut.h"
#include "SinglyLinkedList.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Player;

class Room {
private:
    List<Obstacle*> obstacles;  // List to store pointers to obstacles
    List<Obstacle*> tempObstacle;
    SinglyLinkedList<Character*> characters; // SinglyLinkedList for NPCs like Undead
    string name;
    bool npcsSpawned;
    bool tempStatus;

public:
    // Constructors
    Room(); // Default for initializing rooms with Lists
    Room(string _name);
    ~Room(); // Destructor to destroy all obstacle and characters

    // Getters
    string getName();

    void addObstacle(Obstacle* obstacle);  // Add obstacle to the room
    const List<Obstacle*>& getObstacles() const;  // Return the list of obstacle pointers
    void addTempObstacle(Obstacle* obstacle);  // Add obstacle to the room
    const List<Obstacle*>& getTempObstacles() const;  // Return the list of obstacle pointers

    SinglyLinkedList<Character*>& getCharacters(); // Return the list of character pointers
    void checkPlayerCollisions(Player& player);

    void draw(sf::RenderWindow& window);  // Draw all obstacles and characters
    bool checkKillCollision(const sf::FloatRect& playerBounds) const;
    void update(float deltaTime, Player& player, sf::RenderWindow& window);
    void updateProjectile(float deltaTime, Player& player, sf::RenderWindow& window);
    void resetNPC();
    void resetNPCdead();
    bool areNPCsSpawned() const;
    void setNPCsSpawned(bool spawned);
    void removeObstacle(Obstacle* obstacle);
    bool getTempStatus();
    void setTempStatus(bool status);
};
