#pragma once

#include <SFML/Graphics.hpp>
#include "Queue.h"
#include "Character.h"
#include "Room.h"
#include "Inventory.h"
#include "LazerGun.h"
#include "TimeWinder.h"
#include "Projectile.h"
#include "ItemVisitor.h"
#include "SkillTree.h"
#include "Stack.h"

class Room;
class SkillTree;

class Player : public Character {
private:
    sf::Sprite playerSprite;  // Change to sf::Sprite for texture application
    sf::Texture playerTexture; // Texture to hold the image

    sf::RectangleShape playerShape;
    float speed;
    float jumpHeight;
    float gravity;
    bool isJumping;
    bool isGrounded;  // New boolean variable
    float velocityY;
    Inventory inventory;
    SkillTree* skillTree;  // SkillTree instance for the Player
    bool showSkillTree;

    // Stack to store player position history
    Stack<sf::Vector2f>* playerPositionHistory;
    float positionUpdateTimer;

    float projectileSpeed;
    int experience;
    int selectedItemIndex;
    float damageImmunityTimer; // Timer to track damage immunity
    bool isImmune; // Status to check if player is immune
    bool playerDead;
    float damageReduction;

    // Projectile Queue
    Queue<Projectile> projectiles;

    bool tabPressed = false;  // State of Tab key
    bool spacePressed = false; // State of Space key

    // Projectile
    float facingAngle; // Angle the player is facing

    // Visitor Pointer
    ItemVisitor* V;

public:
    // Constructor
    Player(int maxHP);
    ~Player();

    // Debug method to print position history
    void printPositionHistory() const {
        if (playerPositionHistory != nullptr) {
            for (int i = 0; i < playerPositionHistory->size(); ++i) {
                sf::Vector2f pos = playerPositionHistory->back();
                std::cout << "Position " << i << ": (" << pos.x << ", " << pos.y << ")\n";
            }
        }
    }

    // Getter and setter
    bool getPlayerDead();
    void setPlayerDead(bool isDead);
    void setJumpHeight(float newHeight);
    float getJumpHeight();
    void setSpeed(float newSpeed);
    float getSpeed();
    void setDamageReduction(float newDamageReduction);
    void setProjectileSpeed(float newProjectileSpeed);
    float getProjectileSpeed();
    Stack<sf::Vector2f>* getPlayerPositionHistory();

    // Method to spend experience
    void gainExperience(int aXp);
    int getExperience();
    void spendExperience(int points);
    Queue<Projectile>& getProjectiles();

    // Player-specific methods
    void handleInput();
    void update(float deltaTime, Room& room, sf::RenderWindow& window);  // Update with room collision handling
    void drawInventoryOverlay(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSkillIndexFromMousePosition(int mouseX, int mouseY);

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

    // Player Behaviour Method
    void takeDamage(int damage);
    void respawn();
    void pickUpItemObstacle(Room* currentRoom);

    // Update all projectiles
    void updateProjectiles(float deltaTime);
    void drawProjectiles(sf::RenderWindow& window);
    void drawHealthBar(sf::RenderWindow& window);
    void drawSkillTree(sf::RenderWindow& window);
};
