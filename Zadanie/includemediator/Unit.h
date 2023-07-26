#pragma once
# include <string>
//Base class of units
class Unit {
public:
	//all unit parameters
	int id;
	int attackRange;
	int movementRange;
	int buildTime;
	int health;
	int price;
	int x;
	int y;
	char type;
	char team;
	//constructor
	Unit() : id(0), attackRange(0), movementRange(0), buildTime(0), health(0), price(0), x(0), y(0), team(0), type(0) {}
	//copy constructor
	Unit(const Unit& other);
	//function that returns initials - team and type
	virtual char getInitials() = 0;
	//function that returns unit type
	virtual std::string unitType() = 0;
	//function that uses copy constructor and return copied object
	virtual Unit* clone() const = 0;
	//function that changes location of unit
	virtual void changeLocation(int newX, int newY) = 0;
	//function that makes unit take damage
	virtual void takeDamage(int damage) = 0;
	//function that reduces units movement points
	virtual void reduceMovementPoints(int movementCost) = 0;

};