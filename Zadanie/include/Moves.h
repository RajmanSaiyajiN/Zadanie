#pragma once
#include "Unit.h"
#include "Map.h"
#include "Moves.h"
#include "Status.h"
#include "MapOfUnits.h"
#include "Orders.h"
//header containing functions that are needed to find best move

//function to calculate distance between 2 units
int distanceBetweenUnits(const Unit& unit1, const Unit& unit2);
//function returning amount of damage that attacker will deal to defender based of unit types
int getDamage(char attacker, char defender);
//function returning vector of pairs of all possible moves that unit can make
std::vector<std::pair<int, int>> getPossibleMoves(const Unit& unit, const Map& map, MapOfUnits& mou);
//function that finds best move and adds it to orders
void findBestMoves(const Map& map, MapOfUnits& mou, Status& status, Orders& orders);
//function that calculates best move for single unit
void findBestMove(const Unit& unit, const Map& map,MapOfUnits& mou, Status& status, Orders& orders);
//function that finds best target for all units and orders attack
void findBestAttacks(const Map& map, MapOfUnits& mou, Status& status, Orders& orders);
//function that calculates best unit to produce
void findBestPurchase (const Map& map, MapOfUnits& mou, Status& status, Orders& orders);
//function that returns value of move of worker
int evaluateWorkerMove(const Unit& unit, const Map& map,Status& status, int newX, int newY);
//function that returns value of move of all units but workers
int evaluateMove(const Unit& unit, const Map& map, Status& status, bool isAttacking, bool towardsEnemyBase, bool towardsFriendlyBase, int newX, int newY);
