#include "ItemVisitor.h"
#include "Item.h"
#include "LazerGun.h"
#include "Shovel.h"

// Visitor Visit Method
void ItemVisitor::Visit(Item& item) {}
void ItemVisitor::Visit(LazerGun& lazerGun) {}
void ItemVisitor::Visit(Shovel& shovel) {}

// Use Item Visitor
ItemVisitorUse::ItemVisitorUse(float pX, float pY, float pAngle)
{
	playerX = pX;
	playerY = pY;
	angle = pAngle;
}
void ItemVisitorUse::Visit(Item& item) { item.use(); }
void ItemVisitorUse::Visit(LazerGun& lazerGun) { lazerGun.use(playerX, playerY, angle); }
void ItemVisitorUse::Visit(Shovel& shovel) { shovel.use(); }

// Print Item Visitor
void ItemVisitorPrint::Visit(Item& item) { cout << item.getName() << endl; }
void ItemVisitorPrint::Visit(LazerGun& lazerGun) { cout << lazerGun.getName() << endl; }
void ItemVisitorPrint::Visit(Shovel& shovel) { cout << shovel.getName() << endl; }