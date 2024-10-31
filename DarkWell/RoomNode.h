#pragma once

class Room;  // Forward declaration to avoid circular dependency

class RoomNode {
public:
    Room* room;               // Pointer to the actual Room
    RoomNode* left;           // Pointer to the left RoomNode
    RoomNode* right;          // Pointer to the right RoomNode
    RoomNode* up;             // Pointer to the up RoomNode
    RoomNode* down;           // Pointer to the down RoomNode

    RoomNode(Room* roomPtr)
        : room(roomPtr), left((RoomNode*)0), right((RoomNode*)0), up((RoomNode*)0), down((RoomNode*)0) {}
};
