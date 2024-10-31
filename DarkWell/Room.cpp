#include "Room.h"
#include <iostream>

using namespace std;

Room::Room(string _name) { name = _name; }

Room::Room() {}

Room::~Room() {

}

// Add an obstacle to the room
void Room::addObstacle(Obstacle* obstacle) {
    obstacles.pushBack(obstacle);  // Store the pointer in the list
}

// Draw all obstacles in the room
void Room::draw(sf::RenderWindow& window) {
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        it.getCurrent()->getValue()->draw(window);
        ++it;
    }
}

string Room::getName() { return name; }

// Get the list of obstacles
const List<Obstacle*>& Room::getObstacles() const {
    return obstacles;  // Return the list of pointers to obstacles
}