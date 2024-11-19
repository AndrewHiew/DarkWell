#include "KillObstacle.h"

// Constructor
KillObstacle::KillObstacle(float x, float y, float width, float height)
    : NormalObstacle(x, y, width, height) {
    shape.setFillColor(sf::Color::Red); // Indicate danger with red color
}

// Draw the obstacle
void KillObstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Check if player collides with the obstacle
bool KillObstacle::checkCollision(const sf::FloatRect& playerBounds) const {
    return shape.getGlobalBounds().intersects(playerBounds);
}

float KillObstacle::getWidth() { return width; }
void KillObstacle::setWidth(float aWidth) { width = aWidth; }