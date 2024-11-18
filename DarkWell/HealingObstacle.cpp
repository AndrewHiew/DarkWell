#include "HealingObstacle.h"
#include <iostream>

// Default constructor
HealingObstacle::HealingObstacle()
    : Obstacle(0, 0, 10.f, 10.f) {
    shape.setFillColor(sf::Color::Green); // Green to represent healing
}

// Parameterized constructor
HealingObstacle::HealingObstacle(float x, float y, float width, float height)
    : Obstacle(x, y, width, height) {
    shape.setFillColor(sf::Color::Green); // Green color
}

// Override the onCollision method to restore player's health
void HealingObstacle::onCollision(Player& player) {
    if (player.getCurrentHP() < player.getMaxHP()) {
        player.setCurrentHP(player.getMaxHP()); // Restore health to maximum
        std::cout << "Player's health restored to max!" << std::endl;
    }
}
