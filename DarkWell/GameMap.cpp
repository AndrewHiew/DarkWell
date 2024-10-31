#include "GameMap.h"
#include <stdexcept>
#include <iostream> // for std::cout

// Constructor
GameMap::GameMap() : currentRoom(nullptr) {}

// Destructor
GameMap::~GameMap() {
    // Destructor of Tree will take care of deleting all nodes and rooms
}

// Set the root room of the Map
void GameMap::setRootRoom(Room* room) {
    TreeNode<Room*>* rootNode = new TreeNode<Room*>(room);
    roomTree.setRoot(rootNode);
    currentRoom = room;
}

// Add a room in a specific direction from the parent room
void GameMap::addRoom(Room* parent, Room* room, const std::string& direction) {
    // Find the node corresponding to the parent room
    TreeNode<Room*>* parentNode = roomTree.getRoot();
    if (!parentNode || parentNode == &TreeNode<Room*>::NIL) {
        throw std::invalid_argument("Parent room not found in the tree.");
    }

    // Find the correct index for the direction and add the child node
    int dirIndex = directionIndex(direction);
    std::cout << "Direction Index: " << dirIndex << std::endl;
    TreeNode<Room*>* childNode = new TreeNode<Room*>(room, parentNode);  // Pass parentNode to the child

    // Add the room to the parent node at the given direction index
    if (dirIndex >= 0 && dirIndex < 4) {
        parentNode->addChild(childNode);
    }
    else {
        throw std::invalid_argument("Invalid direction provided.");
    }
}

// Helper function to get directional index
int GameMap::directionIndex(const std::string& direction) const {
    if (direction == "up") return 0;
    if (direction == "down") return 1;
    if (direction == "left") return 2;
    if (direction == "right") return 3;
    return -1; // Invalid direction
}

// Set the current room
void GameMap::setCurrentRoom(Room* room) {
    currentRoom = room;
    std::cout << "Current room loaded: " << room->getName() << std::endl;
}

// Handle transition to another room in a specified direction
void GameMap::handleRoomTransition(const std::string& direction) {
    if (!currentRoom) return;

    int dirIndex = directionIndex(direction);

    if (dirIndex == -1) {
        std::cerr << "Error: Invalid direction for transition." << std::endl;
        return;
    }

    // Access the children of the current room node
    TreeNode<Room*>* currentRoomNode = roomTree.getRoot();

    // Make sure we access the correct child based on the direction
    const auto& children = currentRoomNode->getChildren();
    if (dirIndex < children.size()) {
        TreeNode<Room*>* nextRoomNode = children[dirIndex]; // Correct mapping based on direction

        if (nextRoomNode && nextRoomNode != &TreeNode<Room*>::NIL) {
            currentRoom = nextRoomNode->getValue();
            std::cout << "Transitioned to room: " << currentRoom->getName() << std::endl;
        }
        else {
            std::cerr << "Error: No room exists in the specified direction (" << direction << ")." << std::endl;
        }
    }
    else {
        std::cerr << "Error: No room exists in the specified direction (" << direction << ")." << std::endl;
    }
}


// Get the current room
Room* GameMap::getCurrentRoom() const {
    return currentRoom;
}

// Add obstacles to a room
void GameMap::addObstacle(Room* room, Obstacle* obstacle) {
    if (room) {
        room->addObstacle(obstacle);
    }
}
