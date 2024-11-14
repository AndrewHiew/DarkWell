#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ItemVisitor.h"

class ItemVisitor;

class Item {
protected:
    sf::Color color;  // Color for the item

public:
    virtual std::string getName() const = 0;
    virtual void use() const = 0;
    sf::Color getColor() const { return color; }  // Method to get the item's color

    // Visitor Method
    virtual void accept(ItemVisitor& v) {
        v.Visit(*this);
    }
};
