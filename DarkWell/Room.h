#pragma once
#include "Obstacle.h"
#include "List.h"
#include <SFML/Graphics.hpp>

class Room {
private:
    List<Obstacle> obstacles;  // List to store obstacles

public:
    Room();  // Constructor

    void addObstacle(const Obstacle& obstacle);  // Add obstacle to the room
    const List<Obstacle>& getObstacles() const;
    void draw(sf::RenderWindow& window);  // Draw all obstacles
};
