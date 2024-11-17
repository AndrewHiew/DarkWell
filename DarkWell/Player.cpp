#include "Player.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructor: initializes player attributes
Player::Player(int maxHP)
    : Character(maxHP), speed(200.0f), jumpHeight(500.0f), gravity(1000.0f), isJumping(false), velocityY(0.0f) {
    playerShape.setSize(sf::Vector2f(24, 32));  // Player is 24x32 pixels
    playerShape.setFillColor(sf::Color::Blue);  // Blue color for now
    playerShape.setPosition(100, 300);  // Initial position
    selectedItemIndex = 0; // Initial Item
    facingAngle = 0.0f; // Initial Angle
    isGrounded = true;
    V = nullptr;
    damageImmunityTimer = 0.0f;
    isImmune = false;
    playerDead = false;
    projectiles = Queue<Projectile>(5);
}

Player::~Player() {
    // List class handles the destruction of Items
}

// Method to get the current position of the player
sf::Vector2f Player::getPosition() const {
    return playerShape.getPosition();  
}

// Method to set the current position of the player
void Player::setPosition(float x, float y) {
    playerShape.setPosition(x, y);
}

// Keyboard event handler for player input
void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        playerShape.move(-speed * 0.016f, 0);  // Move left
        facingAngle = 180.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        playerShape.move(speed * 0.016f, 0);  // Move right
        facingAngle = 0.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJumping && isGrounded) {
        velocityY = -jumpHeight;  // Start a jump
        isJumping = true;
    }

    // Handle Tab key as a toggle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !tabPressed) {
        cycleInventory(1);  // Move to the next item
        tabPressed = true;  // Set state to pressed
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        tabPressed = false;  // Reset state when released
    }

    // Handle Space key as a toggle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
        useSelectedItem();  // Use the currently selected item
        spacePressed = true;  // Set state to pressed
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        spacePressed = false;  // Reset state when released
    }
}

std::string Player::vectorToString(const sf::Vector2f& position) {
    std::ostringstream oss;
    oss << "(" << position.x << ", " << position.y << ")";
    return oss.str();
}

// Update method to handle gravity, movement, and collision detection
void Player::update(float deltaTime, Room& room) {
    // Apply damage immunity timer
    if (isImmune) {
        damageImmunityTimer -= deltaTime;
        if (damageImmunityTimer <= 0.0f) {
            isImmune = false; // Reset immunity
        }
    }

    // Apply gravity only if the player is not grounded
    if (!isGrounded) {
        velocityY += gravity * deltaTime;  // Apply gravity
    }

    // Move the player based on vertical velocity (gravity)
    playerShape.move(0, velocityY * deltaTime);

    // Check for collisions with room obstacles
    sf::FloatRect playerBounds = getBounds();

    // Reset grounded status before checking new collisions
    isGrounded = false;

    room.checkPlayerCollisions(*this);

    // Iterate over the room's obstacles to detect collisions
    auto it = room.getObstacles().getIteratorFromFront();
    while (it != it.end()) {
        Obstacle* obstacle = it.getCurrent()->getValue();  // Get pointer to Obstacle
        sf::FloatRect obstacleBounds = obstacle->getBounds();

        // Check collision with KillObstacle (downwards or upwards)
        if (KillObstacle* killObstacle = dynamic_cast<KillObstacle*>(obstacle)) {
            if (playerBounds.intersects(obstacleBounds)) {
                // Handle collision with KillObstacle (red color)
                std::cout << "Player collided with KillObstacle!" << std::endl;
                break;
            }
        }

        // Collision resolution for other obstacles
        if (playerBounds.intersects(obstacleBounds)) {
            // Calculate the offsets for collision resolution
            float overlapTop = playerBounds.top + playerBounds.height - obstacleBounds.top;
            float overlapBottom = obstacleBounds.top + obstacleBounds.height - playerBounds.top;
            float overlapLeft = playerBounds.left + playerBounds.width - obstacleBounds.left;
            float overlapRight = obstacleBounds.left + obstacleBounds.width - playerBounds.left;

            // Resolve the smallest overlap to prevent sticking
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // Collision from the top
                playerShape.setPosition(playerShape.getPosition().x, obstacleBounds.top - playerBounds.height);
                velocityY = 0;  // Reset vertical velocity
                isJumping = false;  // Player is on the ground
                isGrounded = true;  // Player is grounded
            }
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // Collision from the bottom
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

        // Check for the possibility to pick up an item when adjacent to an ItemObstacle
        if (ItemObstacle* itemObstacle = dynamic_cast<ItemObstacle*>(obstacle)) {
            if (playerBounds.intersects(obstacleBounds)) {
                // Display message or prompt if the player is close enough to an ItemObstacle
                std::cout << "Press E to pick up: " << itemObstacle->getItem()->getName() << std::endl;

                // Handle the 'E' key press to grab the item
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    // Add the item to the player's inventory
                    this->addItemToInventory(itemObstacle->getItem());
                    std::cout << "Picked up: " << itemObstacle->getItem()->getName() << std::endl;

                    // Remove the obstacle from the room
                    room.removeObstacle(*it);
                    delete itemObstacle; // Delete obstacle to prevent memory leak
                    break; // Exit loop since the obstacle list has been modified
                }
            }
        }

        ++it;
    }
}

// Player Take Damage Method
void Player::takeDamage(int damage) {
    if (!isImmune) { // Only take damage if not immune
        currentHP -= damage;

        if (currentHP <= 0) {
            // Check Inventory for TOTEM
            if (inventory.findTotem()) {
                cout << "Totem Used" << endl;
                currentHP = 20;
                isImmune = true; // Activate immunity
                damageImmunityTimer = 1.0f; // 1-second immunity
                
                return;
            }

            currentHP = 0; // Prevent negative health
            std::cout << "Player has died!" << std::endl;
            playerDead = true;
        }

        isImmune = true; // Activate immunity
        damageImmunityTimer = 1.0f; // 1-second immunity
        std::cout << "Player took " << damage << " damage! Current HP: " << currentHP << std::endl;
    }
}


// Draws the player in the game window
void Player::draw(sf::RenderWindow& window) {
    window.draw(playerShape); // Draw the Player Character
    drawInventoryOverlay(window); // Draw the Inventory Overlay
}

// Draws the Inventory Overlay
void Player::drawInventoryOverlay(sf::RenderWindow& window) {
    float itemSize = 32.0f; // Size of each item in the inventory
    float spacing = 10.0f;  // Spacing between items

    for (int i = 0; i < inventory.getSize(); ++i) {
        sf::RectangleShape itemShape(sf::Vector2f(itemSize, itemSize));
        itemShape.setFillColor(inventory.getItem(i)->getColor());  // Use color from item

        itemShape.setPosition(10 + i * (itemSize + spacing), 10); // Position each item
        window.draw(itemShape); // Draw item shape

        // Highlight the selected item
        if (i == getSelectedItemIndex()) {
            itemShape.setOutlineThickness(3);
            itemShape.setOutlineColor(sf::Color::Yellow);  // Highlight color for selected item
            window.draw(itemShape); // Draw highlighted item shape
        }
    }
}

// Returns the bounding box of the player for collision detection
sf::FloatRect Player::getBounds() const {
    return playerShape.getGlobalBounds();
}

// Inventory management methods
void Player::addItemToInventory(Item* item) {
    inventory.addItem(item);
}

void Player::cycleInventory(int direction) {
    int size = inventory.getSize();
    if (size > 0) {
        selectedItemIndex = (selectedItemIndex + direction + size) % size;
    }
}

// Method to use the selected Item
void Player::useSelectedItem() {
    if (!inventory.isEmpty()) {
        Item* selectedItem = inventory.getItem(getSelectedItemIndex());

        // Debug print for item usage
        std::cout << "Using item: " << selectedItem->getName() << std::endl;

        if (LazerGun* lazerGun = dynamic_cast<LazerGun*>(selectedItem)) {
            // Call use on the LazerGun, passing player position and facing angle
            V = new ItemVisitorUse(getPosition().x + 10, getPosition().y + 10, facingAngle, this);
            lazerGun->accept(*V);
            delete V;
            V = nullptr;
        }
        else {
            // Fallback for other items if applicable
            V = new ItemVisitorUse(getPosition().x + 10, getPosition().y + 10, facingAngle, this);
            selectedItem->accept(*V);
            delete V;
            V = nullptr;
        }
    }
}

// Update all projectiles
void Player::updateProjectiles(float deltaTime) {
    int size = projectiles.size();
    for (int i = 0; i < size; ++i) {
        projectiles.front().update(deltaTime);
        projectiles.enqueue(projectiles.dequeue());
    }
}

// Draw all projectiles
void Player::drawProjectiles(sf::RenderWindow& window) {
    int size = projectiles.size();
    for (int i = 0; i < size; ++i) {
        projectiles.front().draw(window);
        projectiles.enqueue(projectiles.dequeue());
    }
}


int Player::getSelectedItemIndex() const {
    return selectedItemIndex;
}

const Inventory& Player::getInventory() const {
    return inventory;
}

void Player::respawn() {
    playerShape.setPosition(50, 318); // Reset to initial position
    isJumping = false;  // Player is on the ground
    velocityY = 0.0f;  // Reset vertical velocity
    isGrounded = true;  // Player is grounded
}

void Player::pickUpItemObstacle(Room* currentRoom) {
    for (auto it = currentRoom->getObstacles().begin(); it != currentRoom->getObstacles().end(); ++it) {
        ItemObstacle* itemObstacle = dynamic_cast<ItemObstacle*>(*it);

        if (itemObstacle && this->getBounds().intersects(itemObstacle->getBounds())) {
            // Add the item to the player's inventory
            this->addItemToInventory(itemObstacle->getItem());
            std::cout << "Picked up: " << itemObstacle->getItem()->getName() << std::endl;

            // Remove the obstacle from the room without manually deleting it
            currentRoom->removeObstacle(*it);
            break; // Exit loop since the obstacle list has been modified
        }
    }
}


bool Player::getPlayerDead() { return playerDead; }
void Player::setPlayerDead(bool isDead) { playerDead = isDead; }
Queue<Projectile>& Player::getProjectiles() { return projectiles; }