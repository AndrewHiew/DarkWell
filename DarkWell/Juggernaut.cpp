#include "Juggernaut.h"

// Constructor initializes health, color, and size
Juggernaut::Juggernaut(int maxHP, float posX, float posY)
    : Character(maxHP), speed(100.0f), isDead(false), gravity(1000.0f), velocityY(0.0f), isGrounded(false) {
    juggernautShape.setSize(sf::Vector2f(24, 32));
    juggernautShape.setFillColor(sf::Color(255, 69, 0));  // Temporary color (Orange)
    juggernautShape.setPosition(posX, posY);  // Initial position for the Undead
    initX = posX;
    initY = posY;
}

// Get current position of the Undead
sf::Vector2f Juggernaut::getPosition() const {
    return juggernautShape.getPosition();
}

// Set position of the Undead
void Juggernaut::setPosition(float x, float y) {
    juggernautShape.setPosition(x, y);
}

// Check if the Undead is dead
bool Juggernaut::isDeadCheck() const {
    return isDead;
}

// Get bounding box for collision detection
sf::FloatRect Juggernaut::getBounds() const {
    return juggernautShape.getGlobalBounds();
}

// Draw the Undead character
void Juggernaut::draw(sf::RenderWindow& window) {
    if (!isDead) {
        window.draw(juggernautShape);
    }
}

bool Juggernaut::getIsDead() {
    if (currentHP <= 0) {
        isDead = true;
        return isDead;
    }
    else {
        return isDead;
    }
}

// Simple AI: Move towards the player's position
void Juggernaut::moveTowardsPlayer(sf::Vector2f playerPosition, float deltaTime) {
    sf::Vector2f direction = playerPosition - getPosition();
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize the direction vector
    if (length != 0) {
        direction /= length;
    }

    // Move the Undead towards the player
    juggernautShape.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
}

// Inflict damage to the Undead
void Juggernaut::takeDamage(int damage) {
    currentHP -= damage;
    if (currentHP <= 0) {
        currentHP = 0;
        setIsDead(true);  // Mark the undead as dead
    }
}


// Method to Deal damage to Player
void Juggernaut::checkPlayerCollision(Player& player) {
    if (!isDead && getBounds().intersects(player.getBounds())) {
        player.takeDamage(20); // Deal 20 damage to the player
    }
}


// Update method for the Undead
void Juggernaut::update(float deltaTime, const List<Obstacle*>& obstacles, sf::Vector2f playerPosition) {
    if (!isDead) {
        // Apply gravity if not grounded
        if (!isGrounded) {
            velocityY += gravity * deltaTime;
        }

        // Move vertically based on gravity
        juggernautShape.move(0, velocityY * deltaTime);

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
                    juggernautShape.setPosition(juggernautShape.getPosition().x, obstacleBounds.top - undeadBounds.height);
                    velocityY = 0;  // Stop falling
                    isGrounded = true;
                }
                else {
                    // Collision from below
                    juggernautShape.setPosition(juggernautShape.getPosition().x, obstacleBounds.top + obstacleBounds.height);
                    velocityY = 0;  // Reset vertical velocity
                }
            }
            ++it;
        }

        // Simple AI movement towards player
        moveTowardsPlayer(playerPosition, deltaTime);
    }
}

void Juggernaut::resetNPC() {
    juggernautShape.setPosition(initX, initY);
    currentHP = 500;
}

void Juggernaut::resetNPCdead() {
    juggernautShape.setPosition(initX, initY);
    currentHP = 500;
    isDead = false;
}