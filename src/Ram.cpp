#include "Ram.h"
#include <string>

Ram::Ram() : Unit() {

	type = 'R';

	health = 90;

	movementRange = 2;

	attackRange = 1;

	price = 500;

	buildTime = 4;

}
Ram::Ram(char team, int id, int x, int y, int health) : Ram() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Ram::Ram(const Ram& other) : Unit(other) {

}

std::string Ram::unitType() {

	return "Ram";

}

Ram* Ram::clone() const {
	return new Ram(*this);
}

char Ram::getInitials() {
	return type;
}

void Ram::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Ram::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Ram::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}