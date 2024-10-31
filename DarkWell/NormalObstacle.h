#pragma once
#include "Obstacle.h"

class NormalObstacle : public Obstacle {
public:
    // Default constructor
    NormalObstacle();

    // Parameterized constructor
    NormalObstacle(float x, float y, float width, float height);

    void draw(sf::RenderWindow& window) const override;
};
