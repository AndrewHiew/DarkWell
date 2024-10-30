#include "Map.h"

Map::Map() {
    rootRoom = new Room();  // Start with a single room
}

Map::~Map() {
    delete rootRoom;  // Free the memory
}
