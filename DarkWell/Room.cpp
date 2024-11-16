#include "Room.h"
#include <iostream>

using namespace std;

Room::Room(string _name) { name = _name; }

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

    // Draw characters (NPCs like Undead)
    Node<Character*>* current = characters.getHead();
    while (current != nullptr) {
        Undead* undead = dynamic_cast<Undead*>(current->value);
        if (undead && !undead->getIsDead()) {
            undead->draw(window);
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
void Room::update(float deltaTime, Player& player) {
    // Apply damage when Undead collides with the player
    Node<Character*>* current = characters.getHead();

    while (current != nullptr) {
        Undead* undead = dynamic_cast<Undead*>(current->value);
        if (undead && !undead->getIsDead()) {
            undead->checkPlayerCollision(player);
        }
        current = current->next;
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
