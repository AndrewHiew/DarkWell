#include "Player.h"

// Constructor: initializes player attributes
Player::Player(int maxHP)
    : Character(maxHP), speed(200.0f), jumpHeight(500.0f), gravity(1000.0f), isJumping(false), velocityY(0.0f) {
    playerShape.setSize(sf::Vector2f(24, 32));  // Player is 24x32 pixels
    playerShape.setFillColor(sf::Color::Blue);  // Blue color for now
    playerShape.setPosition(100, 300);  // Initial position
    selectedItemIndex = 0;
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJumping) {
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


// Update method to handle gravity, movement, and collision detection
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
        sf::FloatRect obstacleBounds = obstacle->getBounds();

        if (playerBounds.intersects(obstacleBounds)) {
            // Calculate the offsets for collision
            float overlapTop = playerBounds.top + playerBounds.height - obstacleBounds.top;
            float overlapBottom = obstacleBounds.top + obstacleBounds.height - playerBounds.top;
            float overlapLeft = playerBounds.left + playerBounds.width - obstacleBounds.left;
            float overlapRight = obstacleBounds.left + obstacleBounds.width - playerBounds.left;

            // Determine the smallest overlap for collision resolution
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // Collision from the top
                playerShape.setPosition(playerShape.getPosition().x, obstacleBounds.top - playerBounds.height);
                velocityY = 0;  // Reset vertical velocity
                isJumping = false;  // Player is now on the ground
            }
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // Collision from the bottom
                playerShape.setPosition(playerShape.getPosition().x, obstacleBounds.top + obstacleBounds.height);
                velocityY = 0;
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

// Draws the player in the game window
void Player::draw(sf::RenderWindow& window) {
    window.draw(playerShape);
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
            lazerGun->use(getPosition().x, getPosition().y, facingAngle);
        }
        else {
            // Fallback for other items if applicable
            selectedItem->use();
        }
    }
}


int Player::getSelectedItemIndex() const {
    return selectedItemIndex;
}

const Inventory& Player::getInventory() const {
    return inventory;
}
