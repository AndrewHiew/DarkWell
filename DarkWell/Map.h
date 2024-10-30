#pragma once

#include "Room.h"


class Map {
public:
    Room* rootRoom;  // Root of the tree structure (starting room)

    Map();
    ~Map();  // Destructor to free up memory
};