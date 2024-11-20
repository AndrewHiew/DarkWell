#include "Room.h"
#include "HealingObstacle.h"
#include <iostream>

using namespace std;

Room::Room(string _name) 
{ 
   name = _name; 
   npcsSpawned = false;
   tempStatus = false;
}

Room::Room() {}

Room::~Room() {
    // Clear characters
    while (!characters.isEmpty()) {
        delete characters.getHead()->value;
        characters.popFront();
    }
}

// Add an obstacle to the room
void Room::addObstacle(Obstacle* obstacle) {
    obstacles.pushBack(obstacle);  // Store the pointer in the list
}

const List<Obstacle*>& Room::getObstacles() const {
    return obstacles;  // Return the list of pointers to obstacles
}

void Room::addTempObstacle(Obstacle* obstacle) {
    tempObstacle.pushBack(obstacle);
}

const List<Obstacle*>& Room::getTempObstacles() const {
    return tempObstacle;
}

SinglyLinkedList<Character*>& Room::getCharacters() {
    return characters;
}

bool Room::getTempStatus() { return tempStatus; }
void Room::setTempStatus(bool status) { tempStatus = status; }

// Draw all obstacles in the room
void Room::draw(sf::RenderWindow& window) {
    static sf::Texture backgroundTexture;
    static sf::Sprite backgroundSprite;

    // Load the background texture only once
    static bool isLoaded = false;
    if (!isLoaded) {
        if (!backgroundTexture.loadFromFile("room-1.png")) {
            std::cerr << "Error: Failed to load room-1.png!" << std::endl;
            return;
        }
        backgroundSprite.setTexture(backgroundTexture);
        isLoaded = true;
    }

    // Draw the background
    window.draw(backgroundSprite);

    // Draw obstacles
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        it.getCurrent()->getValue()->draw(window);
        ++it;
    }

    if (!tempStatus) {
        // Draw temp obstacles
        auto it = tempObstacle.getIteratorFromFront();
        while (it != it.end()) {
            it.getCurrent()->getValue()->draw(window);
            ++it;
        }
    }
    else {
        // Safely delete all temp obstacles
        auto it = tempObstacle.getIteratorFromFront();
        while (it != it.end()) {
            delete it.getCurrent()->getValue(); // Free the memory for each obstacle
            it = tempObstacle.erase(it); // Remove the node from the list
        }
    }

    // Draw characters (NPCs)
    Node<Character*>* current = characters.getHead();
    while (current != nullptr) {
        Undead* undead = dynamic_cast<Undead*>(current->value);
        Juggernaut* boss = dynamic_cast<Juggernaut*>(current->value);
        if (undead && !undead->getIsDead()) {
            undead->draw(window);
        }
        if (boss && !boss->getIsDead()) {
            boss->draw(window);
        }
        current = current->next;
    }
}


string Room::getName() { return name; }


void Room::checkPlayerCollisions(Player& player) {
    sf::FloatRect playerBounds = player.getBounds();

    // Check collision with NPCs (Undead, Juggernaut)
    Node<Character*>* current = characters.getHead();
    while (current != nullptr) {
        Undead* undead = dynamic_cast<Undead*>(current->value);
        Juggernaut* boss = dynamic_cast<Juggernaut*>(current->value);

        // Handle collision with Undead
        if (undead && !undead->getIsDead()) {
            undead->update(0.016f, getObstacles(), player.getPosition());
            sf::FloatRect undeadBounds = undead->getBounds();

            // If player intersects with Undead
            if (playerBounds.intersects(undeadBounds)) {
                // Apply damage to the player
                player.takeDamage(20);

                // Resolve player collision with the Undead
                if (playerBounds.left < undeadBounds.left) {
                    player.setPosition(undeadBounds.left - playerBounds.width, player.getPosition().y);
                }
                else if (playerBounds.left + playerBounds.width > undeadBounds.left + undeadBounds.width) {
                    player.setPosition(undeadBounds.left + undeadBounds.width, player.getPosition().y);
                }
                else if (playerBounds.top < undeadBounds.top) {
                    player.setPosition(player.getPosition().x, undeadBounds.top - playerBounds.height);
                }
                else if (playerBounds.top + playerBounds.height > undeadBounds.top + undeadBounds.height) {
                    player.setPosition(player.getPosition().x, undeadBounds.top + undeadBounds.height);
                }
                return;
            }
        }

        // Handle collision with Juggernaut (boss)
        else if (boss && !boss->getIsDead()) {
            boss->update(0.016f, getObstacles(), player.getPosition());
            sf::FloatRect bossBounds = boss->getBounds();

            // If player intersects with Juggernaut
            if (playerBounds.intersects(bossBounds)) {
                // Apply damage to the player
                player.takeDamage(40);

                // Resolve player collision with the Juggernaut
                if (playerBounds.left < bossBounds.left) {
                    player.setPosition(bossBounds.left - playerBounds.width, player.getPosition().y);
                }
                else if (playerBounds.left + playerBounds.width > bossBounds.left + bossBounds.width) {
                    player.setPosition(bossBounds.left + bossBounds.width, player.getPosition().y);
                }
                else if (playerBounds.top < bossBounds.top) {
                    player.setPosition(player.getPosition().x, bossBounds.top - playerBounds.height);
                }
                else if (playerBounds.top + playerBounds.height > bossBounds.top + bossBounds.height) {
                    player.setPosition(player.getPosition().x, bossBounds.top + bossBounds.height);
                }
                return;
            }
        }
        current = current->next;
    }

    // Check collision with obstacles, specifically HealingObstacle
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        Obstacle* obstacle = it.getCurrent()->getValue();
        sf::FloatRect obstacleBounds = obstacle->getBounds();

        if (playerBounds.intersects(obstacleBounds)) {
            if (HealingObstacle* healingObstacle = dynamic_cast<HealingObstacle*>(obstacle)) {
                healingObstacle->onCollision(player);
                std::cout << "Player touched a HealingObstacle!" << std::endl;

                // Optionally remove the obstacle after healing
                removeObstacle(healingObstacle);
                delete healingObstacle;
                break;
            }
        }
        ++it;
    }
}



bool Room::checkKillCollision(const sf::FloatRect& playerBounds) const {
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        if (auto killObstacle = dynamic_cast<KillObstacle*>(it.getCurrent()->getValue())) {
            if (killObstacle->checkCollision(playerBounds)) {
                return true;
            }
        }
        ++it;
    }
    return false;
}

// Method to update the Room Class
void Room::update(float deltaTime, Player& player, sf::RenderWindow& window) {
    // Apply damage when Undead collides with the player
    Node<Character*>* current = characters.getHead();
    Node<Character*>* previous = nullptr;

    while (current != nullptr) {
        // Undead Player Collision Check
        if (dynamic_cast<Undead*>(current->value) != nullptr) {
            Undead* undead = dynamic_cast<Undead*>(current->value);
            if (undead && undead->getIsDead()) {

                // Properly delete character and remove from list
                if (previous) {
                    previous->next = current->next;
                }
                else {
                    characters.setHead(current->next);
                }
                delete current->value;
                delete current;
                current = previous ? previous->next : characters.getHead();
            }
            else {
                undead->checkPlayerCollision(player);
                previous = current;
                current = current->next;
            }
        }
        
        // Juggernaut Player Collision Check
        else if (dynamic_cast<Juggernaut*>(current->value) != nullptr) {
            Juggernaut* boss = dynamic_cast<Juggernaut*>(current->value);
            if (boss && boss->getIsDead()) {

                // Properly delete character and remove from list
                if (previous) {
                    previous->next = current->next;
                }
                else {
                    characters.setHead(current->next);
                }
                delete current->value;
                delete current;
                current = previous ? previous->next : characters.getHead();
            }
            else {
                boss->checkPlayerCollision(player);
                previous = current;
                current = current->next;
            }
        }
    }
}

// Method to updates the projectile, and also adding player XP upon killing enemies.
void Room::updateProjectile(float deltaTime, Player& player, sf::RenderWindow& window) {
    if (!player.getInventory().isEmpty()) {

        player.updateProjectiles(deltaTime);
        player.drawProjectiles(window);

        // Check if the selected item is a LazerGun
        if (LazerGun* lazerGun = dynamic_cast<LazerGun*>(player.getInventory().getItem(player.getSelectedItemIndex()))) {


            // Check for collisions with enemies (NPCs)
            auto& characters = getCharacters();
            int projectileCount = player.getProjectiles().size();

            // Iterate over projectiles
            for (int i = 0; i < projectileCount; ++i) {
                Projectile& projectile = player.getProjectiles().front();
                sf::FloatRect projectileBounds = projectile.getBounds();

                // Iterator for character list
                auto it = characters.getIterator();
                bool projectileHit = false;

                while (it != it.end()) {
                    Character* character = it.getCurrent()->getValue();

                    // Determine if character is an Undead or Juggernaut
                    Undead* undead = dynamic_cast<Undead*>(character);
                    Juggernaut* juggernaut = dynamic_cast<Juggernaut*>(character);

                    // Skip collision check for dead undead characters
                    if (undead && undead->getIsDead()) {
                        ++it;
                        continue;
                    }

                    sf::FloatRect characterBounds = character->getBounds();

                    // If the projectile collides with the character (Undead or Juggernaut)
                    if (projectileBounds.intersects(characterBounds)) {
                        character->takeDamage(20);
                        std::cout << "Projectile hit enemy! Dealt 20 damage." << std::endl;
                        std::cout << "Enemy HP: " << character->getCurrentHP() << std::endl;

                        // Check if the Undead is now dead after taking damage
                        if (undead && undead->getCurrentHP() <= 0 && undead->getIsDead()) {
                            std::cout << "Undead killed! Player gains 3 experience point." << std::endl;
                            player.gainExperience(3); // Increase player's experience by 3
                        }

                        // Check if the Juggernaut is now dead after taking damage
                        if (juggernaut && juggernaut->getCurrentHP() <= 0) {
                            std::cout << "Juggernaut killed! Player gains 10 experience points." << std::endl;
                            player.gainExperience(10); // Increase player's experience by 10
                        }

                        // Remove the projectile after hitting an enemy
                        player.getProjectiles().dequeue();
                        projectileHit = true;
                        break;
                    }

                    ++it;
                }

                // If the projectile did not hit any enemy, re-enqueue it for the next update
                if (!projectileHit) {
                    player.getProjectiles().enqueue(player.getProjectiles().dequeue());
                }
            }
        }
    }
}

// Method to reset all NPC
void Room::resetNPC() {
    Node<Character*>* current = characters.getHead();

    // Iterate through each character in the room
    while (current != nullptr) {
        // Check if the character is of type Undead
        Undead* undead = dynamic_cast<Undead*>(current->value);
        Juggernaut* boss = dynamic_cast<Juggernaut*>(current->value);
        if (undead) {
            undead->resetNPC();
        }
        else if (boss) {
            boss->resetNPC();
        }
        current = current->next;  // Move to the next character
    }
}

void Room::removeObstacle(Obstacle* obstacle) {
    obstacles.remove(obstacle);
}

void Room::resetNPCdead() {
    Node<Character*>* current = characters.getHead();

    // Iterate through each character in the room
    while (current != nullptr) {
        // Check if the character is of type Undead
        Undead* undead = dynamic_cast<Undead*>(current->value);
        Juggernaut* boss = dynamic_cast<Juggernaut*>(current->value);
        if (undead) {
            undead->resetNPCdead(); 
        }
        else if (boss) {
            boss->resetNPCdead();
        }
        current = current->next;  // Move to the next character
    }
}

bool Room::areNPCsSpawned() const { return npcsSpawned; }
void Room::setNPCsSpawned(bool spawned) { npcsSpawned = spawned; }