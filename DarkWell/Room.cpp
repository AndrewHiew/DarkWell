#include "Room.h"
#include <iostream>

using namespace std;

Room::Room(string _name) 
{ 
   name = _name; 
   npcsSpawned = false;
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

SinglyLinkedList<Character*>& Room::getCharacters() {
    return characters;
}

// Draw all obstacles in the room
void Room::draw(sf::RenderWindow& window) {
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        it.getCurrent()->getValue()->draw(window);
        ++it;
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

// Get the list of obstacles
const List<Obstacle*>& Room::getObstacles() const {
    return obstacles;  // Return the list of pointers to obstacles
}

void Room::checkPlayerCollisions(Player& player) {
    sf::FloatRect playerBounds = player.getBounds();

    // Check for collisions with each Undead
    Node<Character*>* current = characters.getHead();
    while (current != nullptr) {
        Undead* undead = dynamic_cast<Undead*>(current->value);
        Juggernaut* boss = dynamic_cast<Juggernaut*>(current->value);

        if (undead && !undead->getIsDead()) {
            undead->update(0.016f, getObstacles(), player.getPosition());

            sf::FloatRect undeadBounds = undead->getBounds();
            if (playerBounds.intersects(undeadBounds)) {
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
                return;  // Stop further checks after resolving one collision
            }
        }

        else if (boss && !boss->getIsDead()) {
            boss->update(0.016f, getObstacles(), player.getPosition());

            sf::FloatRect bossBounds = boss->getBounds();
            if (playerBounds.intersects(bossBounds)) {
                // Resolve player collision with the Undead
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
                return;  // Stop further checks after resolving one collision
            }
        }
        current = current->next;
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


void Room::updateProjectile(float deltaTime, Player& player, sf::RenderWindow& window) {
    if (!player.getInventory().isEmpty()) {
        if (LazerGun* lazerGun = dynamic_cast<LazerGun*>(player.getInventory().getItem(player.getSelectedItemIndex()))) {
            player.updateProjectiles(deltaTime);
            player.drawProjectiles(window);

            // Check for collisions with enemies (NPCs)
            auto& characters = getCharacters();
            int projectileCount = player.getProjectiles().size();

            // Iterate over projectiles
            for (int i = 0; i < projectileCount; ++i) {
                Projectile& projectile = player.getProjectiles().front();
                sf::FloatRect projectileBounds = projectile.getBounds();

                // Check for collision with each character (enemy)
                auto it = characters.getIterator();
                bool projectileHit = false;

                while (it != it.end()) {
                    Character* character = it.getCurrent()->getValue();

                    // Check if character is still alive
                    Undead* undead = dynamic_cast<Undead*>(character);
                    if (undead && undead->getIsDead()) {
                        ++it;
                        continue; // Skip collision check for dead characters
                    }

                    sf::FloatRect characterBounds = character->getBounds();

                    // If the projectile collides with the enemy
                    if (projectileBounds.intersects(characterBounds)) {
                        character->takeDamage(20);
                        std::cout << "Projectile hit enemy! Dealt 20 damage." << std::endl;
                        std::cout << "Enemy HP: " << character->getCurrentHP() << std::endl;

                        // Remove the projectile after hitting the enemy
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
        if (undead) {
            undead->resetNPC();  // Call the resetNPC method of the Undead class
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
        if (undead) {
            undead->resetNPCdead();  // Call the resetNPC method of the Undead class
        }
        current = current->next;  // Move to the next character
    }
}

bool Room::areNPCsSpawned() const { return npcsSpawned; }
void Room::setNPCsSpawned(bool spawned) { npcsSpawned = spawned; }