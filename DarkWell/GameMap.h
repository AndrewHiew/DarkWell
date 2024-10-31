#pragma once
#include "Room.h"
#include "TreeNode.h"
#include "Tree.h"

class GameMap {
private:
    Tree<Room*> roomTree;  // Tree structure to hold rooms
    Room* currentRoom;     // Pointer to the current room

public:
    GameMap();             // Constructor
    ~GameMap();            // Destructor
    void setRootRoom(Room* room);  // Set the root room
    void addRoom(Room* parent, Room* room, const std::string& direction); // Add a room
    void setCurrentRoom(Room* room); // Set the current room
    Room* getCurrentRoom() const; // Get the current room
    void handleRoomTransition(const std::string& direction); // Handle room transitions
    void handleBackwardTransition(); // Handle backward transition
    void addObstacle(Room* room, Obstacle* obstacle); // Add obstacle to room
    int directionIndex(const std::string& direction) const; // Get direction index
    void findParentNode(TreeNode<Room*>* node, Room* parent, TreeNode<Room*>*& foundNode);

    TreeNode<Room*>* findRoomNode(Room* room);
};