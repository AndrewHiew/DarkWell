#include "ItemObstacle.h"

// Default constructor
ItemObstacle::ItemObstacle() 
    : Obstacle(0, 0, 10.f, 10.f), item(nullptr) {
    shape.setFillColor(sf::Color(128, 0, 128)); // Purple
}

// Parameterized constructor
ItemObstacle::ItemObstacle(float x, float y, Item* aItem) 
    : Obstacle(x, y, 10.f, 10.f), item(aItem) {
    shape.setFillColor(aItem->getColor());
}

// Getter for the item's name
Item* ItemObstacle::getItem() {
    return item;
}

// Override the draw method to render the item obstacle (10x10 purple square)
void ItemObstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);  // Draw the obstacle as a purple square
}
