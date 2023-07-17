#include "Unit.h"

Unit::Unit(const Unit& other) {
	this->id = other.id;
	this->attackRange = other.attackRange;
	this->movementRange = other.movementRange;
	this->buildTime = other.buildTime;
	this->health = other.health;
	this->price = other.price;
	this->x = other.x;
	this->y = other.y;
	this->team = other.team;
	this->type = other.type;
}