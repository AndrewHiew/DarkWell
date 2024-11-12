#pragma once
#include "NormalObstacle.h"
#include "MovingObstacle.h"
#include "KillObstacle.h"
#include "List.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Room {
private:
    List<Obstacle*> obstacles;  // List to store pointers to obstacles
    string name;

public:
    // Constructors
    Room(); // Default for initializing rooms with Lists
    Room(string _name);  
    ~Room(); // Destructor to destroy all obstacle first

    // Getters
    string getName();

    void addObstacle(Obstacle* obstacle);  // Add obstacle to the room
    const List<Obstacle*>& getObstacles() const;  // Return the list of obstacle pointers
    void draw(sf::RenderWindow& window);  // Draw all obstacles
    bool checkKillCollision(const sf::FloatRect& playerBounds) const;
};
