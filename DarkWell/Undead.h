#pragma once
#include "Character.h"
#include "Obstacle.h"
#include "Player.h"
#include "List.h"
#include "Tree.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Player;

class Undead : public Character {
private:
    sf::RectangleShape undeadShape; // Shape for the Undead character
    float speed;  // Speed of movement
    bool isDead;  // Health status
    float gravity;     // Gravity value same as player's
    float velocityY;   // Vertical velocity for gravity
    bool isGrounded;   // To check if the Undead is grounded
    float initX;
    float initY;

public:
    // Constructor with default health value
    Undead(int maxHP, float posX, float posY);

    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    bool getIsDead();
    bool isDeadCheck() const;
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window);
    void takeDamage(int damage);

    void checkPlayerCollision(Player& player);
    void update(float deltaTime, const List<Obstacle*>& obstacles, sf::Vector2f playerPosition);
    void moveTowardsPlayer(sf::Vector2f playerPosition, float deltaTime);
    void resetNPC();
    void resetNPCdead();
};
