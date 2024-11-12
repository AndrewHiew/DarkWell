#pragma once

#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Room.h"
#include "Inventory.h"
#include "LazerGun.h"
#include "Shovel.h"
#include "Projectile.h"

class Player : public Character {
private:
    sf::RectangleShape playerShape;
    float speed;
    float jumpHeight;
    float gravity;
    bool isJumping;
    bool isGrounded;  // New boolean variable
    float velocityY;
    Inventory inventory;
    int selectedItemIndex;

    bool tabPressed = false;  // State of Tab key
    bool spacePressed = false; // State of Space key

    // Projectile
    List<Projectile> projectiles; // List to hold projectiles
    float facingAngle; // Angle the player is facing

public:
    // Constructor
    Player(int maxHP);
    ~Player();

    // Player-specific methods
    void handleInput();
    void update(float deltaTime, Room& room);  // Update with room collision handling
    void drawInventoryOverlay(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    // Position and movement-related methods
    sf::FloatRect getBounds() const;  // Get bounding box for collision
    void setPosition(float x, float y);  // Set player position
    sf::Vector2f getPosition() const;  // Get current position of player
    std::string vectorToString(const sf::Vector2f& position);

    // Inventory functions
    void addItemToInventory(Item* item);  // Add item to inventory
    void cycleInventory(int direction);   // Cycle through inventory
    void useSelectedItem();               // Use selected item
    int getSelectedItemIndex() const;     // Get index of currently selected item
    const Inventory& getInventory() const; // Get inventory (const reference)
    void respawn();
};
