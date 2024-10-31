#include "NormalObstacle.h"

// Default constructor
NormalObstacle::NormalObstacle() : Obstacle() {}

// Parameterized constructor
NormalObstacle::NormalObstacle(float x, float y, float width, float height)
    : Obstacle(x, y, width, height) {
    shape.setFillColor(sf::Color::Green);  // Color the normal obstacle green
}

// Draw method implementation
void NormalObstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
