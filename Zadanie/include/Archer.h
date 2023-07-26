#pragma once
#include "Unit.h"
class Archer : public Unit {

public:

	Archer();

	Archer(char team, int id, int x, int y, int health);

	Archer(const Archer& other);

	std::string unitType() override;

	Archer* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

};