#include "Obstacle.h"

Obstacle::Obstacle() {
    shape.setSize(sf::Vector2f(0, 0));
    shape.setPosition(0, 0);
    shape.setFillColor(sf::Color::Transparent);  // Default invisible obstacle
}

Obstacle::Obstacle(float x, float y, float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);  // Red color for visibility
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);  // Draw the rectangle shape
}

const sf::RectangleShape& Obstacle::getShape() const {
    return shape;
}

sf::FloatRect Obstacle::getBounds() const {
    return shape.getGlobalBounds();
}
