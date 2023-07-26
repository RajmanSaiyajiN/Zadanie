#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "MediatorOrders.h"
#include "MediatorMap.h"
#include "Base.h"
#include "ProductionControl.h"

	int getMovement(char unitType){
		std::string movementValues[2][7] = { //tab containing unit types and its movement points
			{"K","S","A","P","C","R","W"},
			{"5", "2", "2", "2", "2", "2", "2"},
		};
		
		int i=0, x=0;
		while (i < 7 && x == 0) {
			if (unitType == movementValues[0][i][0]) {
				x = i;
				break;
			}
			else {
				i++;
			}
		}
		return std::stoi(movementValues[1][x]);
		
	}

	int getDamage(char attacker, char defender) {
		std::string damageValues[8][9] = { //tab containing unit types and damage
			{"0","K","S","A","P","C","R","W","B"},
			{"K", "35", "35", "35", "35", "35", "50", "35", "35"},
			{"S", "30", "30", "30", "20", "20", "30", "30", "30"},
			{"A", "15", "15", "15", "15", "10", "10", "15", "15"},
			{"P", "35", "15", "15", "15", "15", "10", "15", "10"},
			{"C", "40", "40", "40", "40", "40", "40", "40", "50"},
			{"R", "10", "10", "10", "10", "10", "10", "10", "50"},
			{"W", "5", "5", "5", "5", "5", "5", "5", "1"}
		};
		int i = 0, j = 0;
		int x = 0, y = 0;
		//find both types in tab
		while (i < 8 && x == 0) {
			if (attacker == damageValues[i][0][0]) {
				x = i;
			}
			else {
				i++;
			}
		}
		while (j < 9 && y == 0) {
			if (defender == damageValues[0][j][0]) {
				y = j;
			}
			else {
				j++;
			}
		}
		//read data and return as int
		if (x != 0 && y != 0) {
			return std::stoi(damageValues[x][y]);
		}
		//return 0 if types not found
		else {
			return 0;
		}

	}

	int getDamageById(int attacker, int defender, Status& status) {
		std::vector<Unit*> allUnits = status.getUnits();
		char attackerType, defenderType;
		for (const auto& someUnit : allUnits) {
			if (someUnit->id == attacker) {
				attackerType = someUnit->type;
			}
			else if (someUnit->id == defender) {
				defenderType = someUnit->type;
			}
		}
		return getDamage(attackerType, defenderType);
	}
	
	int getProductionTime(char type){
		std::string productionTime[2][7] = { //tab containing unit types and damage
			{"K","S","A","P","C","R","W"},
			{"5", "3", "3", "3", "6", "4", "2"}
		};
		
		int i=0, x=0;
		while (i < 7 && x == 0) {
			if (type == productionTime[0][i][0]) {
				x = i;
				break;
			}
			else {
				i++;
			}
		}
		return std::stoi(productionTime[1][x]);
	}

	bool Orders::checkID(int id, Status& status) {
		std::vector<Unit*> allUnits = status.getUnits();
		for (const auto& someUnit : allUnits) {
			if (someUnit->id == id) {
				return true;
			}
		}
		return false;
	}

	bool Orders::canMove(int id, Status& status) {
		std::vector<Unit*> allUnits = status.getUnits();
		for (const auto& someUnit : allUnits) {
			if (someUnit->id == id) {
				if (someUnit->type == 'B') {
					return false;
				}
			}
		}
		return true;
	}


	bool Orders::hasMovementPoints(int id, Status& status,int y, int x) {
		std::vector<Unit*> allUnits = status.getUnits();
		for (const auto& someUnit : allUnits) {
			if (someUnit->id == id) {
				someUnit->movementRange=getMovement(someUnit->type);
				//std::cout<<"MV check: " << someUnit->id<< " "<< someUnit->type<< " "<<someUnit->x<<" "<<someUnit->y<<"xy "<< x<< " " << y<<" "<<someUnit->movementRange<<std::endl;
				if ((std::abs(x - someUnit->x) + std::abs(y - someUnit->y))<someUnit->movementRange) {
					return true;
				}
				else if ((std::abs(x - someUnit->x) + std::abs(y - someUnit->y)) == someUnit->movementRange) {
					someUnit->movementRange = 0;
					return true; 
				}
			}
		}
		return false;
	}

	bool Orders::checkDestinationPoint(int id, Status& status, Map& map, int y, int x) {
		std::vector<Unit*> allUnits = status.getUnits();
		int mapX = map.getX();
		int mapY = map.getY();
		for (const auto& someUnit : allUnits) {
			if (someUnit->x == x && someUnit->y == y) {
				return false;
			}
		}
		if(mapX>x && mapY>y && x >=0 && y >=0){
				if (map.getTile(y, x) == 6 || map.getTile(y, x)==0) {
					return true;
				}
			}
		return false;
	}

	

	bool Orders::checkTeam(int id, int enemyId, Status& status) {
		std::vector<Unit*> allUnits = status.getUnits();
		char team1;
		char team2;
		for (const auto& someUnit : allUnits) {
			if (someUnit->id == id) {
				team1 = someUnit->team;
			}
			else if (someUnit->id == enemyId) {
				team2 = someUnit->team;
			}
		}
		if (team1 == team2) {
			return false;
		}
		return true;
	}

	bool Orders::checkBaseProduction(int id, Status& status) {
		std::vector<Unit*> allUnits = status.getUnits();
		for (const auto& friendlyUnit : allUnits) {
			//find base
			if (friendlyUnit->team == 'P' && friendlyUnit->type == 'B') {
				if (Base* baseUnit = dynamic_cast<Base*>(friendlyUnit)) {
					if (baseUnit->checkProduction() == false);
					return true;
				}
			}
		}
		return false;
	}

	bool Orders::canAttack(int id, int enemyId, Status& status) {
		std::vector<Unit*> allUnits = status.getUnits();
		int unit1X, unit1Y, unit2X, unit2Y;
		bool hasRange = false;
		int attackRange = 0;
		for (const auto& someUnit : allUnits) {
			if (someUnit->id == id) {
				unit1X = someUnit->x;
				unit1Y = someUnit->y;
				attackRange = someUnit->attackRange;
				if (someUnit->movementRange >= 1) {
					hasRange = true;
				}

			}
			else if (someUnit->id == enemyId) {
				unit2X = someUnit->x;
				unit2Y = someUnit->y;
			}
		}
		if ((std::abs(unit1X - unit2X) + std::abs(unit1Y - unit2Y)) <= attackRange && hasRange==true) {
			return true;
		}
		return false;
	}

	Orders::Orders() {

	}

	Orders::Orders(std::string filename) {
		loadFromFile(filename);

	}

	void Orders::loadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (file.is_open()) {
			std::vector<std::string> lines; //vector of lines
			std::string line;
			while (std::getline(file, line)) {//read all lines 
				if (!line.empty()) {
					//std::cout<<line<<std::endl;
					orders.push_back(line);  //add line to vector if it contains some data
				}
			}
		}
		else {
			//std::cerr << "Błąd otwarcia pliku: " << filename << std::endl;
		}
		file.close();
	}

	bool Orders::checkConditions() {
		return true;
	}

	void Orders::addOrder(const int unitId, char action, int x, int y) {
		std::string order;
		if (checkConditions()) {
			order = std::to_string(unitId) + " " + action + " " + std::to_string(x) + " " + std::to_string(y);
			orders.push_back(order);
		}
	}

	void Orders::addOrder(int unitId, char action, int enemyUnitId) {
		std::string order;
		if (checkConditions()) {
			order = std::to_string(unitId) + " " + action + " " + std::to_string(enemyUnitId);
			orders.push_back(order);
		}
	}

	void Orders::addOrder(int unitId, char base, char production) {
		std::string order;
		if (checkConditions()) {
			order = std::to_string(unitId) + " " + base + " " + production;
			orders.push_back(order);
		}
	}

	void Orders::saveToFile(const std::string& filename) {
		std::ofstream file(filename);

		if (file.is_open()) {
			for (const std::string& order : orders) {
				file << order << std::endl;
			}

			file.close();
			//std::cout << "Orders saved to file: " << filename << std::endl;
		}
		else {
			//std::cout << "Unable to open file: " << filename << std::endl;
		}
	}

	void Orders::checkOrders(Status& status, Map& map,ProductionControl& controller) {
		for (int i = 0; i < (orders.size()); i++) {
			//std::cout<<"Enter order: "<<orders[i]<<std::endl;
			std::istringstream iss(orders[i]);  //helps load data divided by space directly to variable type
			int id;
			char orderType;
			int  x, y;
			char production;
			int enemyID;

		
			if (iss >> id >> orderType >> x >> y) {
				//std::cout<<"Move passed"<<std::endl;
				if (orderType == 'M') {
					//std::cout<<"Move is move: "<< checkID(id, status)<<" "<< canMove(id, status)<<" "<< hasMovementPoints(id, status, y, x)<<" " << checkDestinationPoint(id, status, map, y, x)<<std::endl;
					if (checkID(id, status) == true && canMove(id, status) == true && hasMovementPoints(id, status, y, x) == true && checkDestinationPoint(id, status, map, y, x) == true) {
						status.changeLocation(id, x, y);
						//std::cout<<"Move doneyooooo"<<id<<" "<< x << " " << y << std::endl;
						continue;
					}

				}
			}
			else {
				//std::cout<<"Move not doneyooooo"<<id<<" "<< x << " " << y << std::endl;
				iss.clear();        //clear stream
				iss.seekg(0);       //set reading pointer to 0 - reset it

			}
			//attack order
			if (iss >> id >> orderType >> enemyID) {
				//std::cout<<"attack passed"<<std::endl;
				if (orderType == 'A') {
					//std::cout<<"attack is attack: "<< checkID(id, status) <<" " << checkID(enemyID, status)<<" "<< canMove(id, status) << " "<< canAttack(id, enemyID, status)<<std::endl;
					if (checkID(id, status) == true && checkID(enemyID, status) == true && canMove(id, status) && checkTeam(id, enemyID, status) == true && canAttack(id, enemyID, status) == true) {
						status.getDamage(enemyID, getDamageById(id, enemyID, status));
						//std::cout<<"attack made: "<<id<<" "<< enemyID << std::endl;
						continue;
					}
				}
			}
			else {
				//std::cout <<"Bladyataku"<< orders[i] << std::endl;
				iss.clear();        //clear stream
				iss.seekg(0);       //set reading pointer to 0 - reset it

			}
			//production order
			if (iss >> id >> orderType >> production) {
				if (orderType == 'B') {
					//std::cout<<"found buy order"<<std::endl;
					if (checkID(id, status) == true && checkBaseProduction(id, status) == true) {
						status.setProduction(id, production);
						controller.type = production;
						controller.remainingTime=getProductionTime(production);
						//std::cout<<"Production added"<<id<<" "<< orderType << std::endl;
						continue;
					}
				}
			}
			else {
				std::cout<<"Dq";
				exit(0);
			}
		}
	}

