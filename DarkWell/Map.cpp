#include "Map.h"

Map::Map() {
    // Create the root RoomNode and its associated rooms
    rootRoom = new RoomNode(new Room());

    // Create left and right RoomNodes
    rootRoom->left = new RoomNode(new Room());
    rootRoom->right = new RoomNode(new Room());

    // Set neighbors
    rootRoom->left->right = rootRoom;   // Connect left RoomNode back to root
    rootRoom->right->left = rootRoom;   // Connect right RoomNode back to root
}

Map::~Map() {
    // Clean up all rooms and room nodes
    delete rootRoom->left->room; 
    delete rootRoom->right->room;
    delete rootRoom->left;
    delete rootRoom->right;
    delete rootRoom;
}
