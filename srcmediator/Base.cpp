#include "Base.h"
#include <string>

Base::Base() : Unit() {

	type = 'B';

	health = 200;

	movementRange = 0;

	attackRange = 0;

	price = 0;

	buildTime = 0;

	production = '0';

}
Base::Base(char team, int id, int x, int y, int health) : Base() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Base::Base(char team, int id, int x, int y, int health, char production):Base(team, id, x, y, health) {
	this->production = production;
}

Base::Base(const Base& other) : Unit(other) {
	this->production = other.production;
}

std::string Base::unitType() {

	return "Base";

}

Base* Base::clone() const {
	return new Base(*this);
}

char Base::getInitials() {
	return type;
}

void Base::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Base::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Base::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}

void Base::setProduction(char newProduction) {
		this->production = newProduction;
}

bool Base::checkProduction() {
	if (this->production == '0') {
		return false;
	}
	else {
		return true;
	}
}

char Base::getProductionType() {
	return this->production;
}
