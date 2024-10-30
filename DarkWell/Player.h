#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Room.h"

class Player : public Character {
private:
    sf::RectangleShape playerShape;
    float speed;
    float jumpHeight;
    float gravity;
    bool isJumping;
    float velocityY;

public:
    Player(int maxHP);

    void handleInput();
    void update(float deltaTime, Room& room);  // Update with room collision handling
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;  // Bounding box for collision
};
