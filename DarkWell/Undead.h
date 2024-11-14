#pragma once
#include "Character.h"
#include "Obstacle.h"
#include "List.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Undead : public Character {
private:
    sf::RectangleShape undeadShape;  // Shape for the Undead character
    float speed;  // Speed of movement
    bool isDead;  // Health status
    float gravity;     // Gravity value same as player's
    float velocityY;   // Vertical velocity for gravity
    bool isGrounded;   // To check if the Undead is grounded

public:
    // Constructor with default health value
    Undead(int maxHP, float posX, float posY);

    // Get position of the Undead
    sf::Vector2f getPosition() const;

    // Set position of the Undead
    void setPosition(float x, float y);

    // Getter for isDead
    bool getIsDead();

    // Check if the Undead is dead
    bool isDeadCheck() const;

    // Get bounding box for collision detection
    sf::FloatRect getBounds() const;

    // Movement method (simple AI behavior)
    void moveTowardsPlayer(sf::Vector2f playerPosition, float deltaTime);

    // Draw the Undead character
    void draw(sf::RenderWindow& window);

    // Update health status
    void takeDamage(int damage);

    void update(float deltaTime, const List<Obstacle*>& obstacles, sf::Vector2f playerPosition);
};
