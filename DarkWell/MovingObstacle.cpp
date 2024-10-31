#include "MovingObstacle.h"

// Constructor
MovingObstacle::MovingObstacle(float x, float y, float width, float height, float speed, float minY, float maxY)
    : NormalObstacle(x, y, width, height), speed(speed), minY(minY), maxY(maxY), movingUp(true) {
}

void MovingObstacle::update(float deltaTime) {
    float currentY = shape.getPosition().y;

    // Move the obstacle up and down
    if (movingUp) {
        shape.move(0, -speed * deltaTime);
        if (currentY <= minY) {
            movingUp = false;  // Change direction when reaching the top
        }
    }
    else {
        shape.move(0, speed * deltaTime);
        if (currentY >= maxY) {
            movingUp = true;  // Change direction when reaching the bottom
        }
    }

    // Keep the obstacle within the bounds of Room 1 and Room 3
    if (currentY < minY) {
        shape.setPosition(shape.getPosition().x, minY);
        movingUp = false;
    }
    else if (currentY > maxY) {
        shape.setPosition(shape.getPosition().x, maxY);
        movingUp = true;
    }
}

// Method to check if the obstacle is at its upper limit
bool MovingObstacle::isAtUpperLimit() const {
    return shape.getPosition().y <= minY;
}

// Draw the moving obstacle
void MovingObstacle::draw(sf::RenderWindow& window) const {
    NormalObstacle::draw(window);
}
