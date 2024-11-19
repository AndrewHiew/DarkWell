#pragma once
#include "NormalObstacle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class KillObstacle : public NormalObstacle {
public:
    // Constructor
    KillObstacle(float x, float y, float width, float height);

    // Draw the obstacle
    void draw(sf::RenderWindow& window) const override;

    // Check if player collides with the obstacle
    bool checkCollision(const sf::FloatRect& playerBounds) const;

    float getWidth();
    void setWidth(float aWidth);
};
