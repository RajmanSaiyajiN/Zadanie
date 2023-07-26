#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Orders.h"

	//bool Orders::checkID(int id) {

	//}

//	bool Orders::checkMovementPoints() {}

//	bool Orders::checkDestinationPoint() {}

//	bool Orders::checkTeam() {}

	//bool Orders::checkBaseProduction() {}

	Orders::Orders() {

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
				//std::cout<<order<<std::endl;
				file << order << std::endl;
			}

			//std::cout << "Orders saved to file: " << filename << std::endl;
		}
		else {
			//std::cout << "Unable to open file: " << filename << std::endl;
		}
		file.close();
	}
