#include "Player.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructor: initializes player attributes
// Player.cpp
Player::Player(int maxHP)
    : Character(maxHP),
    speed(200.0f),
    jumpHeight(500.0f),
    gravity(1000.0f),
    isJumping(false),
    velocityY(0.0f),
    isGrounded(true),
    V(nullptr),
    damageImmunityTimer(0.0f),
    isImmune(false),
    playerDead(false),
    projectiles(Queue<Projectile>(5)),
    experience(100),
    projectileSpeed(500.0f),
    damageReduction(1.0f),
    showSkillTree(false)
{
    // Retaining the rectangle shape with size 24x32
    playerShape.setSize(sf::Vector2f(24, 32));  // Shape size
    playerShape.setPosition(100, 300);          // Initial position

    // Load the player texture
    if (!playerTexture.loadFromFile("player.png")) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }

    // Apply the texture to the rectangle shape
    playerShape.setTexture(&playerTexture);  // Apply the texture to the shape

    // Optionally, set texture rect to control the part of the texture being shown, if needed
    // playerShape.setTextureRect(sf::IntRect(0, 0, 24, 32));  // Optional: set the specific texture region if needed

    selectedItemIndex = 0;  // Initial Item
    facingAngle = 0.0f;
    skillTree = new SkillTree(*this);
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
void Player::update(float deltaTime, Room& room, sf::RenderWindow& window) {
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

    // Temp Obstacle Collision
    auto it2 = room.getTempObstacles().getIteratorFromFront();
    while (it2 != it2.end()) {
        Obstacle* obstacle = it2.getCurrent()->getValue();  // Get pointer to Obstacle
        sf::FloatRect obstacleBounds = obstacle->getBounds();
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
        it2++;
    }
    draw(window);
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
                
                selectedItemIndex = 0;
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
    drawHealthBar(window); // Draw the health bar
    drawSkillTree(window); // Draw the skill tree

    // Create a sf::Text object to display the coordinates
    sf::Text coordsText;
    sf::Font font;

    // Load font (ensure the font file path is correct)
    if (!font.loadFromFile("Arial.ttf")) {
        // Handle error if font loading fails
        return;
    }

    // Set the text string with player's coordinates
    std::ostringstream coordsStream;
    coordsStream << "X: " << playerShape.getPosition().x << " Y: " << playerShape.getPosition().y;
    coordsText.setString(coordsStream.str());

    // Set text properties
    coordsText.setFont(font);
    coordsText.setCharacterSize(14); // Set the font size
    coordsText.setFillColor(sf::Color::White); // Set text color
    coordsText.setStyle(sf::Text::Regular);

    // Position the text at the bottom-left corner
    coordsText.setPosition(10, window.getSize().y - 20); // Adjust the y-position for some margin from the window's bottom

    // Draw the coordinates text
    window.draw(coordsText);
}


// Draws skill tree
void Player::drawSkillTree(sf::RenderWindow& window) {
    static bool tabPressed = false; // Flag to track if the T key press was processed

    // Toggle skill tree visibility when 'T' is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !tabPressed) {
        tabPressed = true;          // Mark the T key as pressed
        showSkillTree = !showSkillTree; // Toggle the visibility of the skill tree
    }

    // Reset the flag when the T key is released
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        tabPressed = false;
    }

    // If the skill tree is visible, draw it
    if (showSkillTree) {
        skillTree->drawSkillTree(window);
    }

    // Handle mouse clicks to select a skill if the skill tree is visible
    if (showSkillTree && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        int skillIndex = getSkillIndexFromMousePosition(mousePosition.x, mousePosition.y);
        if (skillIndex != -1) {
            skillTree->handleSkillSelection(skillIndex);
        }
    }
}

int Player::getSkillIndexFromMousePosition(int mouseX, int mouseY) {
    int startX = 100;  // Starting X position
    int startY = 50;   // Starting Y position for the root node
    int nodeWidth = 150;
    int nodeHeight = 50;
    int horizontalSpacing = 180; // Between nodes at the same level
    int verticalSpacing = 50;    // Between different levels

    // Root node
    if (mouseX >= startX && mouseX <= startX + nodeWidth &&
        mouseY >= startY && mouseY <= startY + nodeHeight) {
        return 0; // Root skill
    }

    // First-level nodes
    for (int i = 0; i < 2; ++i) {
        int nodeX = startX + (i * horizontalSpacing);
        int nodeY = startY + verticalSpacing;

        if (mouseX >= nodeX && mouseX <= nodeX + nodeWidth &&
            mouseY >= nodeY && mouseY <= nodeY + nodeHeight) {
            return i + 1; // Skill index for first-level nodes
        }

        // Second-level nodes under each first-level node
        for (int j = 0; j < 2; ++j) {
            int childNodeX = nodeX + (j * 100); // Position for second-level children
            int childNodeY = nodeY + verticalSpacing;

            if (mouseX >= childNodeX && mouseX <= childNodeX + nodeWidth &&
                mouseY >= childNodeY && mouseY <= childNodeY + nodeHeight) {
                if (i == 0 && j == 0) return 3; // "Increase Speed"
                if (i == 1 && j == 0) return 4; // "Reduce Damage"
                if (i == 1 && j == 1) return 5; // "Increase Projectile Speed"
            }
        }
    }

    return -1; // No skill node clicked
}


// Draws the Inventory Overlay
void Player::drawInventoryOverlay(sf::RenderWindow& window) {
    float itemSize = 32.0f; // Size of each item in the inventory
    float spacing = 10.0f;  // Spacing between items

    for (int i = 0; i < inventory.getSize(); ++i) {
        Item* currentItem = inventory.getItem(i);

        // Explicit nullptr check
        if (currentItem == nullptr) {
            std::cout << "Item is nullptr at index " << i << std::endl;
            continue;
        }

        // Load the texture for the item
        sf::Texture texture;
        if (!texture.loadFromFile(currentItem->getImagePath())) {
            std::cerr << "Error: Failed to load image for item at index " << i
                << " (" << currentItem->getImagePath() << ")" << std::endl;
            continue;
        }

        // Create the sprite for the item and set its size
        sf::Sprite itemSprite;
        itemSprite.setTexture(texture);
        itemSprite.setScale(
            itemSize / texture.getSize().x,
            itemSize / texture.getSize().y
        );
        itemSprite.setPosition(10 + i * (itemSize + spacing), 10);

        // Draw the item sprite
        window.draw(itemSprite);

        // Draw the yellow border if the item is selected
        if (i == getSelectedItemIndex()) {
            sf::RectangleShape outline(sf::Vector2f(itemSize, itemSize));
            outline.setPosition(10 + i * (itemSize + spacing), 10);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(3);
            outline.setOutlineColor(sf::Color::Yellow);
            window.draw(outline);
        }
    }
}


// Draws the Health Bar
void Player::drawHealthBar(sf::RenderWindow& window) {
    float heartSize = 20.0f;      // Size of each heart
    float spacing = 5.0f;         // Spacing between hearts
    float startX = 10.0f;         // Starting X position for the hearts
    float startY = 50.0f;         // Y position right under the inventory overlay

    // Calculate the number of full hearts based on current health
    int fullHearts = currentHP / 20;  // 20 health per heart

    // Draw each heart
    for (int i = 0; i < maxHP/20; ++i) {  // Max 3 hearts, since max health is 60
        sf::RectangleShape heartShape(sf::Vector2f(heartSize, heartSize));
        heartShape.setPosition(startX + i * (heartSize + spacing), startY);

        if (i < fullHearts) {
            heartShape.setFillColor(sf::Color::Red);  // Full heart
        }
        else {
            heartShape.setFillColor(sf::Color(100, 0, 0));  // Dark red for empty heart
        }

        window.draw(heartShape);
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

// Method to increase player experience
void Player::gainExperience(int aXp) {
    experience += aXp;
}

// Method to spend experience
void Player::spendExperience(int points) {
    experience -= points;
}

// Getters
int Player::getExperience() {
    return experience;
}

void Player::setJumpHeight(float newHeight) { jumpHeight = newHeight; }
float Player::getJumpHeight() { return jumpHeight; }
void Player::setSpeed(float newSpeed) { speed = newSpeed; }
float Player::getSpeed() { return speed; }
void Player::setDamageReduction(float newDamageReduction) { damageReduction = newDamageReduction; }
void Player::setProjectileSpeed(float newProjectileSpeed) { projectileSpeed = newProjectileSpeed; }
float Player::getProjectileSpeed() { return projectileSpeed; }

bool Player::getPlayerDead() { return playerDead; }
void Player::setPlayerDead(bool isDead) { playerDead = isDead; }
Queue<Projectile>& Player::getProjectiles() { return projectiles; }