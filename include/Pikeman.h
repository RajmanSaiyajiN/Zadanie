#pragma once
#include "Unit.h"
class Pikeman : public Unit {

public:

	Pikeman();

	Pikeman(char team, int id, int x, int y, int health);

	Pikeman(const Pikeman& other);

	std::string unitType() override;

	Pikeman* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

}; 