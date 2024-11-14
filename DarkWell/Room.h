#pragma once
#include "NormalObstacle.h"
#include "MovingObstacle.h"
#include "KillObstacle.h"
#include "List.h"
#include "SinglyLinkedList.h"
#include "Player.h"
#include "Undead.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Player;

class Room {
private:
    List<Obstacle*> obstacles;  // List to store pointers to obstacles
    SinglyLinkedList<Character*> characters; // SinglyLinkedList for NPCs like Undead
    string name;

public:
    // Constructors
    Room(); // Default for initializing rooms with Lists
    Room(string _name);
    ~Room(); // Destructor to destroy all obstacle and characters

    // Getters
    string getName();

    void addObstacle(Obstacle* obstacle);  // Add obstacle to the room
    const List<Obstacle*>& getObstacles() const;  // Return the list of obstacle pointers

    SinglyLinkedList<Character*>& getCharacters(); // Return the list of character pointers
    void checkPlayerCollisions(Player& player);

    void draw(sf::RenderWindow& window);  // Draw all obstacles and characters
    bool checkKillCollision(const sf::FloatRect& playerBounds) const;
    void update(float deltaTime, Player& player);
};
