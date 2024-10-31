#pragma once
#include "NormalObstacle.h"

class MovingObstacle : public NormalObstacle {
private:
    float speed;    // Speed of the moving obstacle
    float minY;    // Minimum Y position
    float maxY;    // Maximum Y position
    bool movingUp; // Direction of movement

public:
    // Constructor
    MovingObstacle(float x, float y, float width, float height, float speed, float minY, float maxY);

    // Update the position of the obstacle
    void update(float deltaTime);

    // Check if the obstacle is at its upper limit
    bool isAtUpperLimit() const;

    // Override draw method if necessary (inherited from NormalObstacle)
    void draw(sf::RenderWindow& window) const override;
};
