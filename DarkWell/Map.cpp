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
    delete rootRoom->left->room;  // Delete the room in the left RoomNode
    delete rootRoom->right->room; // Delete the room in the right RoomNode
    delete rootRoom->left;         // Delete the left RoomNode
    delete rootRoom->right;        // Delete the right RoomNode
    delete rootRoom;               // Finally, delete the root RoomNode
}
