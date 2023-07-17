#pragma once
#include "Unit.h"
class Base : public Unit {
private:
	char production;  //parameter that only base contains

public:

	Base();

	Base(char team, int id, int x, int y, int health);

	Base(char team, int id, int x, int y, int health,char production);

	Base(const Base& other);

	std::string unitType() override;

	Base* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;
	//function that sets production of base
	void setProduction(char newProduction);
	//function that checks if base is producing
	bool checkProduction();
	//function that gets production type of base
	char getProductionType();

};