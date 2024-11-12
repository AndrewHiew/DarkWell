#include "Obstacle.h"

// Default constructor
Obstacle::Obstacle() {
    shape.setSize(sf::Vector2f(50, 50));  // Default size
    shape.setFillColor(sf::Color::Red);   // Default color
}

// Parameterized constructor
Obstacle::Obstacle(float x, float y, float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red); // Default color for obstacles
}

// Draw the obstacle
void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Get the shape of the obstacle
const sf::RectangleShape& Obstacle::getShape() const {
    return shape;
}

// Set position for the obstacle
void Obstacle::setPosition(float x, float y) {
    shape.setPosition(x, y); // Set position of the shape
}

// Get position of the obstacle
sf::Vector2f Obstacle::getPosition() const{
    return shape.getPosition();
}


// Get the bounds for collision detection
sf::FloatRect Obstacle::getBounds() const {
    return shape.getGlobalBounds();
}
