#pragma once
#include "Obstacle.h"

class NormalObstacle : public Obstacle {
public:
    sf::Texture* brickTexture; // Static texture shared across all obstacles

    // Default constructor
    NormalObstacle();
    ~NormalObstacle();

    // Parameterized constructor
    NormalObstacle(float x, float y, float width, float height);

    // Overload
    NormalObstacle(float x, float y, float width, float height, sf::Color color);

    void draw(sf::RenderWindow& window) const override;
};
