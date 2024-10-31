#pragma once
#include "Room.h"
#include "RoomNode.h"

class Map {
public:
    RoomNode* rootRoom; // Change to RoomNode to hold room connections

    Map();
    ~Map();
};
