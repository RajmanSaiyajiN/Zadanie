#pragma once
#include "MediatorStatus.h"
#include "MediatorMap.h"
#include "ProductionControl.h"

int getMovement(char unitType);

int getDamage(char attacker, char defender);

int getDamageById(int attacker, int defender, Status& status);

int getProductionTime(char type);

class Orders {
private:
	std::vector<std::string> orders;

public:
	Orders();

	Orders(std::string filename);

	void loadFromFile(const std::string& filename);

	bool checkConditions();

	bool checkID(int id, Status& status);

	bool canMove(int id, Status& status);

	bool hasMovementPoints(int id, Status& status, int y, int x);

	bool checkDestinationPoint(int id, Status& status, Map& map, int y, int x);

	bool checkTeam(int id, int enemyId, Status& status);

	bool checkBaseProduction(int id, Status& status);

	bool canAttack(int id, int enemyId, Status& status);

	void addOrder(const int unitId, char action, int x, int y);

	void addOrder(int unitId, char action, int enemyUnitId);

	void addOrder(int unitId, char base, char production);

	void saveToFile(const std::string& filename);

	void checkOrders(Status& status, Map& map,ProductionControl& controller);

	
};
