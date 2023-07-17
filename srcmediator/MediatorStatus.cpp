#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "MediatorStatus.h"
#include "Units.h"

	//function that creates new unit other than base
	Unit* Status::createUnit(char team, char type, int id, int x, int y, int health) {

		switch (type) { //new unit created based on type of unit
		case 'K':
			return new Knight(team, id, x, y, health);
		case 'S':
			return new Swordsman(team, id, x, y, health);
		case 'A':
			return new Archer(team, id, x, y, health);
		case 'P':
			return new Pikeman(team, id, x, y, health);
		case 'C':
			return new Catapult(team, id, x, y, health);
		case 'R':
			return new Ram(team, id, x, y, health);
		case 'W':
			return new Worker(team, id, x, y, health);
		default:
			//std::cerr << "Nieznany typ jednostki: " << type << std::endl;
			return nullptr;
		}
	}
	//function that creates new base if there is production argument
	Unit* Status::createUnit(char team, char type, int id, int x, int y, int health,char production) {
		if (type == 'B') {  //make sure its base type
			return new Base(team, id, x, y, health, production);
		}
		else {
			return nullptr;
		}
	}
	//function that opens status file and gets data from it
	void Status::loadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (file.is_open()) {
			std::string line;  //1st line contains gold amount
			if (std::getline(file, line)) {
				gold = std::stol(line);
			}
			else {
				//std::cerr << "Pusty plik: " << filename << std::endl;
				return;
			}

			while (std::getline(file, line)) { //every other line contains information about one unit
				std::istringstream iss(line);  //helps load data divided by space directly to variable type
				char team;
				char type;
				int id, x, y, health;
				char baseProducing;

				if (iss >> team >> type >> id >> x >> y >> health >> baseProducing) {
					Unit* unit = createUnit(team, type, id, x, y, health, baseProducing);
					if (unit != nullptr) {
						units.push_back(unit);
					}
				}
				else {
					iss.clear();        //clear stream
					iss.seekg(0);       //set reading pointer to 0 - reset it

				}
				if (iss >> team >> type >> id >> y >> x >> health) {
					Unit* unit = createUnit(team, type, id, y, x, health);
					if (unit != nullptr) {
						units.push_back(unit);
					}
				}
				/* //part of testing streams, so they work correctly, unnecessary in finished program
				else if (iss.fail()) {
					//std::cout << "strumien is bad";
				}
				else if (iss.good()) {
					//std::cout << "strumien is good";
				}
				else {
					//std::cerr << "blad wczytywania danych jednostki z pliku: " << filename << std::endl;
				}*/
			}

			//std::cout << "Dane statusowe wczytane z pliku." << std::endl;
		}
		else {
			//std::cerr << "Blad otwarcia pliku: " << filename << std::endl;
		}
	}
	//constructor
	Status::Status() :gold(0), sizeX(0), sizeY(0) {

	}
	//constructor that gets data from status file
	Status::Status(const std::string& filename) :Status() {

		loadFromFile(filename);
	}

	Status::~Status(){
		for (int i = 0; i < units.size(); i++) {
			delete units[i];
		}
	}
	//function returning all units that are on board to read
	const std::vector<Unit*>& Status::getUnits() const {
		return units;
	}
	//function to display list of units - testing only
	void Status::displayStatus() const {
		//std::cout << "Ilosc zlota: " << gold << std::endl;
		//std::cout << "Jednostki:" << std::endl;
		for (const Unit* unit : units) {
			//std::cout << "Przynależność: " << ", Typ: " << unit->type << ", Pozycja: (" << unit->x << ", " << unit->y << ")" << std::endl;
		}
	}
	//function to change location of units
	void Status::changeLocation(int unit_id, int newX, int newY) {
		int i = 0;
		while (unit_id != units[i]->id && i >= units.size()) { //find unit by its id
			i++;
		}
		int  distance;
		if (i < units.size()) { //replace old location with new one
			distance = std::abs(units[i]->x - newX) + std::abs(units[i]->y - newY);
			units[i]->x = newX;
			units[i]->y = newY;
			reduceMovementPoints(unit_id, distance);
		}
	}

	void Status::getDamage(int unit_id, int damage) {
		int i = 0;
		while (unit_id != units[i]->id && i >= units.size()) { //find unit by its id
			i++;
		}
		if (i < units.size()) { 
			units[i]->takeDamage(damage);  //call units function
			if (units[i]->health <= 0) {
				delete units[i];
				units.erase(units.begin() + i);
			}
		}
	}

	int Status::getUnitParameterValue(const char type, int parameter) {
		const std::string parametersTable[7][6] = {  //tab containing statistics of units
			// type         health   movmentRange  cost     attack range  producing time
			{ "K",          "70",       "5",      "400",        "1",         "5"           },
			{ "S",          "60",       "2",      "250",        "1",         "3"           },
			{ "A",          "40",       "2",      "250",        "5",         "3"           },
			{ "P",          "50",       "2",      "200",        "2",         "3"           },
			{ "R",          "90",       "2",      "500",        "1",         "4"           },
			{ "C",          "50",       "2",      "800",        "7",         "6"           },
			{ "W",          "20",       "2",      "100",        "1",         "2"           }
		};
		//get type from parameter
		int rowIndex = -1;
		switch (type) {
		case 'K': rowIndex = 0; break;
		case 'S': rowIndex = 1; break;
		case 'A': rowIndex = 2; break;
		case 'P': rowIndex = 3; break;
		case 'R': rowIndex = 4; break;
		case 'C': rowIndex = 5; break;
		case 'W': rowIndex = 6; break;
		default:  return 0; // Typ jednostki nieznany
		}
		//get value from tab
		const std::string& valueStr = parametersTable[rowIndex][parameter];
		//return as int if value is not "-"
		return (valueStr == "-") ? 0 : std::stoi(valueStr);


	}

	void Status::setProduction(int id, char type) {
		int i = 0;
		while (units[i]->id != id) {
			i++;
		}
		if (i < units.size()) {
			//find base
			if (units[i]->team == 'P' && units[i]->type == 'B') {
				if (Base* baseUnit = dynamic_cast<Base*>(units[i])) {
					baseUnit->setProduction(type);
					int cost = getUnitParameterValue(type, 3);
					gold -= cost;
				}
			}
		}
	}



	void Status::reduceMovementPoints(int unit_id, int amount) {
		int i = 0;
		while (unit_id != units[i]->id && i >= units.size()) { //find unit by its id
			i++;
		}
		if (i < units.size()) {
			units[i]->reduceMovementPoints(amount);
		}
	}

	void Status::mirrorStatus(int gold) {
		this->gold = gold;
		for (int i = 0; i < units.size(); i++) {
			if (units[i]->team == 'E') {
				units[i]->team = 'P';
			}
			else {
				units[i]->team = 'E';
			}

		}
	}

	//function to get amount of gold
	int Status::getGold() {
		return gold;
	}
	

	void Status::setGold(int newGold) {
		gold = newGold;
	}
	
	void Status::saveToFile(std::string filename) {
		std::ofstream file(filename);

		if (file.is_open()) {
			file << gold << std::endl;

			for (auto& unit : units) {
				if (unit->type == 'B') {
					if (Base* baseUnit = dynamic_cast<Base*>(unit)) {
						file << baseUnit->team << " " << baseUnit->type << " " << baseUnit->id << " " << baseUnit->x << " " << baseUnit->y << " " << baseUnit->health << " " << baseUnit->getProductionType() << std::endl;
					}
				}
				else {
					file << unit->team << " " << unit->type << " " << unit->id << " " << unit->x << " " << unit->y << " " << unit->health << std::endl;
				}
			}

			file.close();
			//std::cout << "Orders saved to file: " << filename << std::endl;
		}
		else {
			//std::cout << "Unable to open file: " << filename << std::endl;
		}
	}


