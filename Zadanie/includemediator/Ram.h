#pragma once
#include "Unit.h"
class Ram : public Unit {

public:

	Ram();

	Ram(char team, int id, int x, int y, int health);

	Ram(const Ram& other);

	std::string unitType() override;

	Ram* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

};