#pragma once
class Orders {
private:
	std::vector<std::string> orders;

public:
	Orders();

	bool checkConditions();

	void addOrder(const int unitId, char action, int x, int y);

	void addOrder(int unitId, char action, int enemyUnitId);

	void addOrder(int unitId, char base, char production);

	void saveToFile(const std::string& filename);
};