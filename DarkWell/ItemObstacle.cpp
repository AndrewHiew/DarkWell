#include "ItemObstacle.h"

// Default constructor
ItemObstacle::ItemObstacle() 
    : Obstacle(0, 0, 10.f, 10.f), name("Default Item") {
    shape.setFillColor(sf::Color(128, 0, 128)); // Purple color
}

// Parameterized constructor
ItemObstacle::ItemObstacle(float x, float y, const std::string& itemName) 
    : Obstacle(x, y, 10.f, 10.f), name(itemName) {
    shape.setFillColor(sf::Color(128, 0, 128)); // Purple color
}

// Getter for the item's name
std::string ItemObstacle::getName() const {
    return name;
}

// Override the draw method to render the item obstacle (10x10 purple square)
void ItemObstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);  // Draw the obstacle as a purple square
}
