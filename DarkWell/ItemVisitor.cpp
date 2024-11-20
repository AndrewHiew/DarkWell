#include "ItemVisitor.h"
#include "Item.h"
#include "LifeTotem.h"
#include "LazerGun.h"
#include "TimeWinder.h"
#include "Player.h"

// Visitor Visit Method
void ItemVisitor::Visit(Item& item) {}
void ItemVisitor::Visit(LazerGun& lazerGun) {}
void ItemVisitor::Visit(TimeWinder& aTimeWinder) {}
void ItemVisitor::Visit(LifeTotem& aLifeTotem) {}

// Use Item Visitor
ItemVisitorUse::ItemVisitorUse(float pX, float pY, float pAngle, Player* aPlayer)
{
	playerX = pX;
	playerY = pY;
	angle = pAngle;
	player = aPlayer;
}
void ItemVisitorUse::Visit(Item& item) { item.use(); }
void ItemVisitorUse::Visit(LazerGun& lazerGun) { lazerGun.use(playerX, playerY, angle, player); }
void ItemVisitorUse::Visit(TimeWinder& aTimeWinder) { aTimeWinder.use(player); }
void ItemVisitorUse::Visit(LifeTotem& aLifeTotem) { aLifeTotem.use(); }

// Print Item Visitor
void ItemVisitorPrint::Visit(Item& item) { cout << item.getName() << endl; }
void ItemVisitorPrint::Visit(LazerGun& lazerGun) { cout << lazerGun.getName() << endl; }
void ItemVisitorPrint::Visit(TimeWinder& aTimeWinder) { cout << aTimeWinder.getName() << endl; }
void ItemVisitorPrint::Visit(LifeTotem& aLifeTotem) { cout << aLifeTotem.getName() << endl; }