#include "Knight.h"
#include <string>

Knight::Knight() : Unit() {

	type = 'K';

	health = 70;

	movementRange = 5;

	attackRange = 1;

	price = 400;

	buildTime = 5;

}
Knight::Knight(char team, int id, int x, int y, int health) : Knight() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Knight::Knight(const Knight& other) : Unit(other) {

}

std::string Knight::unitType() {

	return "Knight";

}

Knight*  Knight::clone() const {
	return new Knight(*this);
}

char Knight::getInitials() {
	return type;
}

void Knight::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Knight::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Knight::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}