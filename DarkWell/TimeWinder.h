#pragma once
#include "Item.h"
#include <iostream>

class ItemVisitor;

class TimeWinder : public Item {
public:
    TimeWinder() {
        color = sf::Color(139, 69, 19);
        imagePath = "timewinder.png";
    }

    std::string getName() const override { return "TimeWinder"; }
    void use() const override { std::cout << "Using Timewinder.\n"; }

    void use(Player* player);

    virtual void accept(ItemVisitor& v) {
        v.Visit(*this);
    }
};
