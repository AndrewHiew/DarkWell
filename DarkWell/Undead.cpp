#include "Undead.h"

// Constructor initializes health, color, and size
Undead::Undead(int maxHP, float posX, float posY)
    : Character(maxHP), speed(80.0f), isDead(false), gravity(1000.0f), velocityY(0.0f), isGrounded(false) {
    undeadShape.setSize(sf::Vector2f(24, 32));  // Same size as the player
    undeadShape.setPosition(posX, posY);  // Initial position for the Undead

    // Load the player texture
    if (!undeadTexture.loadFromFile("undead.png")) {
        std::cerr << "Failed to load undead texture!" << std::endl;
    }

    // Apply the texture to the rectangle shape
    undeadShape.setTexture(&undeadTexture);  // Apply the texture to the shape

    initX = posX;
    initY = posY;
}

// Get current position of the Undead
sf::Vector2f Undead::getPosition() const {
    return undeadShape.getPosition();
}

// Set position of the Undead
void Undead::setPosition(float x, float y) {
    undeadShape.setPosition(x, y);
}

// Check if the Undead is dead
bool Undead::isDeadCheck() const {
    return isDead;
}

// Get bounding box for collision detection
sf::FloatRect Undead::getBounds() const {
    return undeadShape.getGlobalBounds();
}

// Draw the Undead character
void Undead::draw(sf::RenderWindow& window) {
    if (!isDead) {
        window.draw(undeadShape);
    }
}

bool Undead::getIsDead() {
    if (currentHP <= 0) {
        isDead = true;
        return isDead;
    }
    else {
        return isDead;
    }
}

// Simple AI: Move towards the player's position
void Undead::moveTowardsPlayer(sf::Vector2f playerPosition, float deltaTime) {
    sf::Vector2f direction = playerPosition - getPosition();
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize the direction vector
    if (length != 0) {
        direction /= length;
    }

    // Move the Undead towards the player
    undeadShape.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
}

// Inflict damage to the Undead
void Undead::takeDamage(int damage) {
    currentHP -= damage;
    if (currentHP <= 0) {
        currentHP = 0;
        setIsDead(true);  // Mark the undead as dead
    }
}


// Method to Deal damage to Player
void Undead::checkPlayerCollision(Player& player) {
    if (!isDead && getBounds().intersects(player.getBounds())) {
        player.takeDamage(20); // Deal 20 damage to the player
    }
}


// Update method for the Undead
void Undead::update(float deltaTime, const List<Obstacle*>& obstacles, sf::Vector2f playerPosition) {
    if (!isDead) {
        // Apply gravity if not grounded
        if (!isGrounded) {
            velocityY += gravity * deltaTime;
        }

        // Move vertically based on gravity
        undeadShape.move(0, velocityY * deltaTime);

        // Check collision with obstacles and resolve
        sf::FloatRect undeadBounds = getBounds();
        isGrounded = false;  // Reset grounded status

        auto it = obstacles.getIteratorFromFront();
        while (it != it.end()) {
            Obstacle* obstacle = it.getCurrent()->getValue();
            sf::FloatRect obstacleBounds = obstacle->getBounds();

            if (undeadBounds.intersects(obstacleBounds)) {
                // Calculate overlaps
                float overlapTop = undeadBounds.top + undeadBounds.height - obstacleBounds.top;
                float overlapBottom = obstacleBounds.top + obstacleBounds.height - undeadBounds.top;

                if (overlapTop < overlapBottom) {
                    // Resolve collision from above
                    undeadShape.setPosition(undeadShape.getPosition().x, obstacleBounds.top - undeadBounds.height);
                    velocityY = 0;  // Stop falling
                    isGrounded = true;
                } else {
                    // Collision from below
                    undeadShape.setPosition(undeadShape.getPosition().x, obstacleBounds.top + obstacleBounds.height);
                    velocityY = 0;  // Reset vertical velocity
                }
            }
            ++it;
        }

        // Simple AI movement towards player
        moveTowardsPlayer(playerPosition, deltaTime);
    }
}

void Undead::resetNPC() {
    undeadShape.setPosition(initX, initY);
    currentHP = 100;
}

void Undead::resetNPCdead() {
    undeadShape.setPosition(initX, initY);
    currentHP = 100;
    isDead = false;
}