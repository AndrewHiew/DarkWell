#include "Player.h"

Player::Player(int maxHP)
    : Character(maxHP), speed(200.0f), jumpHeight(500.0f), gravity(400.0f), isJumping(false), velocityY(0.0f) {
    playerShape.setSize(sf::Vector2f(24, 32));  // Player is 24x32 pixels
    playerShape.setFillColor(sf::Color::Blue);  // Blue color for now
    playerShape.setPosition(100, 300);  // Initial position
}

// Method to Return the current position of the player
sf::Vector2f Player::getPosition() const {
    return playerShape.getPosition();  
}

// Method to set the current position of the player
void Player::setPosition(float x, float y) {
    playerShape.setPosition(x, y);
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        playerShape.move(-speed * 0.016f, 0);  // Move left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        playerShape.move(speed * 0.016f, 0);  // Move right
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJumping) {
        velocityY = -jumpHeight;  // Start a jump
        isJumping = true;
    }
}

void Player::update(float deltaTime, Room& room) {
    // Apply gravity
    velocityY += gravity * deltaTime;
    playerShape.move(0, velocityY * deltaTime);

    // Check for collisions with room obstacles
    sf::FloatRect playerBounds = getBounds();

    // Iterate over the room's obstacles to detect collisions
    auto it = room.getObstacles().getIteratorFromFront();
    while (it != it.end()) {
        Obstacle* obstacle = it.getCurrent()->getValue();  // Get the pointer to Obstacle
        sf::FloatRect obstacleBounds = obstacle->getBounds();  // Use arrow operator

        if (playerBounds.intersects(obstacleBounds)) {
            // Calculate the offsets
            float overlapTop = playerBounds.top + playerBounds.height - obstacleBounds.top;
            float overlapBottom = obstacleBounds.top + obstacleBounds.height - playerBounds.top;
            float overlapLeft = playerBounds.left + playerBounds.width - obstacleBounds.left;
            float overlapRight = obstacleBounds.left + obstacleBounds.width - playerBounds.left;

            // Determine the smallest overlap
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // Collision from the top
                playerShape.setPosition(playerShape.getPosition().x, obstacleBounds.top - playerBounds.height);
                velocityY = 0;  // Reset vertical velocity
                isJumping = false;  // Player is now on the ground
            }
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // Collision from the bottom (not resetting velocity here since we want to fall)
                playerShape.setPosition(playerShape.getPosition().x, obstacleBounds.top + obstacleBounds.height);
                velocityY = 0;  // Reset vertical velocity
            }
            else if (overlapLeft < overlapRight) {
                // Collision from the left
                playerShape.setPosition(obstacleBounds.left - playerBounds.width, playerShape.getPosition().y);
            }
            else {
                // Collision from the right
                playerShape.setPosition(obstacleBounds.left + obstacleBounds.width, playerShape.getPosition().y);
            }
        }
        ++it;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerShape);
}

sf::FloatRect Player::getBounds() const {
    return playerShape.getGlobalBounds();  // Return the bounding box of the player
}
