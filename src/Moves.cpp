#include <vector>
#include <cmath>
#include "Unit.h"
#include "Map.h"
#include "Moves.h"
#include "Status.h"
#include "MapOfUnits.h"
#include "Orders.h"
#include "Base.h"

int getUnitParameterValue(const char type,int parameter){
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

int distanceBetweenUnits(const Unit& unit1, const Unit& unit2) {
    return std::abs(unit1.x - unit2.x) + std::abs(unit1.y - unit2.y);
}

std::vector<std::pair<int, int>> getPossibleMoves(const Unit& unit, const Map& map, MapOfUnits& mou) {
    int movementRange = unit.movementRange;

    std::vector<std::pair<int, int>> possibleMoves; //vector of all possible moves

    //check all tiles in radius
    for (int dx = -movementRange; dx <= movementRange; dx++) {
        for (int dy = -movementRange; dy <= movementRange; dy++) {
            if (dx == 0 && dy == 0) {
                continue;
            }
           
            int newX = unit.x + dx; //x coordinate on new move
            int newY = unit.y + dy; //y coordinate on new move
            //check if new tile is in range
            if (std::abs(newX - unit.x) + std::abs(newY - unit.y) <= unit.movementRange) {
                //check if new tile is on map
                if (newX >= 0 && newX < map.getX() && newY >= 0 && newY < map.getY()) {
                    //check if possible to move in that terrain
                    if (map.getTile(newY, newX) == 0 || map.getTile(newY, newX) == 6) {
                        //check if any unit is not on that tile
                        if (mou.getTile(newY, newX) == nullptr) {
                            //if all conditions met, add to vector
                            possibleMoves.push_back(std::make_pair(newY, newX));
                        }
                    }
                }
            }
        }
    }

    return possibleMoves;
}

void findBestMoves(const Map& map, MapOfUnits& mou, Status& status, Orders& orders) {
    std::vector<Unit*> allUnits = status.getUnits(); //vector of all units
    for (const auto& friendlyUnit : allUnits) {
        if (friendlyUnit->team == 'P' && friendlyUnit->type != 'B') { //only friendly units that are not base
            findBestMove(*friendlyUnit, map, mou, status, orders);    //function calculating best move for one unit
        }
    }
}

void findBestMove(const Unit& unit, const Map& map, MapOfUnits& mou,  Status& status, Orders& orders) {
    std::pair<int, int> bestMove; //pair of x,y coordinates
    int bestEval = 0;             //evaluation of best move
    auto allMoves = getPossibleMoves(unit, map, mou); //all possible moves from function
    for (std::pair<int, int> move : allMoves) {
        //if worker
        if (unit.type == 'W') {
            //use worker function to evaluate move value
            int eval = evaluateWorkerMove(unit, map, status, move.first,move.second);
            //save best evaluation and move
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
        //if any other unit
        else {
            bool isAttacking = false;   //can unit attack this round

            if (std::abs(move.second - unit.x) + std::abs(move.first - unit.y) < unit.movementRange) {
                isAttacking = true;  //true if at least 1 movement point left
            }

            std::vector<Unit*> allUnits = status.getUnits();
            int i = 0;
            //find enemy base
            while (allUnits[i]->team != 'E' && allUnits[i]->type != 'B') {
                i++;
            }
            //check if moving towards enemy base
            bool towardsEnemyBase = false;
            int distanceToOpponentBaseBefore = std::abs(allUnits[i]->x - unit.x) + std::abs(allUnits[i]->y - unit.y);
            int distanceToOpponentBaseAfter = std::abs(allUnits[i]->x - move.second) + std::abs(allUnits[i]->y - move.first);
            if (distanceToOpponentBaseAfter < distanceToOpponentBaseBefore) {
                towardsEnemyBase = true;
            }
            //use function to evaluate move value
            int eval = evaluateMove(unit, map, status, isAttacking,towardsEnemyBase,!towardsEnemyBase,move.first,move.second );
            //save best evaluation and move
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }
    //if any move possible, order best, update status, update map of units
    if (allMoves.size() != 0) {
        orders.addOrder(unit.id, 'M', bestMove.second, bestMove.first);
        status.changeLocation(unit.id, bestMove.second, bestMove.first);
        mou.changeLocation(unit.x, unit.y, bestMove.second, bestMove.first);
    }
}

void findBestAttacks(const Map& map, MapOfUnits& mou, Status& status, Orders& orders){
    std::vector<Unit*> allUnits = status.getUnits();
    for (const auto& friendlyUnit : allUnits) {
        //check all friendly units that are not base
        if (friendlyUnit->team == 'P' && friendlyUnit->type !='B') {
            //find all possible targets
            std::vector<Unit*> possibleTargets;
            for (const auto& opponentUnit : allUnits) {
                //check if enemies are in range
                if (opponentUnit->team == 'E' && opponentUnit->health>0 && std::abs(opponentUnit->x - friendlyUnit->x) + std::abs(opponentUnit->y - friendlyUnit->y) <= friendlyUnit->attackRange) {
                    possibleTargets.push_back(opponentUnit); //add possible target
                }
            }
            int iterator=0;
            int mostDamage = 0;
            int bestTargetId=0;
            //if any possible target, find best based on damage dealt
            if (possibleTargets.size() > 0) {
                for (int i = 0; i < possibleTargets.size(); i++) {
                    if (possibleTargets[i]->health>0 && (getDamage(friendlyUnit->type, possibleTargets[i]->type)) > mostDamage) {
                        mostDamage = getDamage(friendlyUnit->type, possibleTargets[i]->type);
                        bestTargetId = possibleTargets[i]->id;
                    }
                }
                //make best move, and update damage
                orders.addOrder(friendlyUnit->id, 'A', bestTargetId);
                status.getDamage(bestTargetId, mostDamage);
            }

        }
    }
}

void findBestPurchase(const Map& map, MapOfUnits& mou, Status& status, Orders& orders){
    std::vector<Unit*> allUnits = status.getUnits();
    Base* myBase;
    bool isWorker = false;
    //find any worker
    for (const auto& friendlyUnit : allUnits) {
        if (friendlyUnit->team == 'P' && friendlyUnit->type == 'W') {
            isWorker = true;
        }
        //find base
        if (friendlyUnit->team == 'P' && friendlyUnit->type == 'B') {
            if (Base* baseUnit = dynamic_cast<Base*>(friendlyUnit)) {
                if (baseUnit->checkProduction() == false) {
                    //weights to find best formula
                    const double costWeight = 1.0;
                    const double movementRangeWeight = 0.5;
                    const double attackRangeWeight = 0.5;
                    const double damageToOpponentBaseWeight = 2.0;
                    const double goldOnMapWeight = 1.0;
                    const double workerCountWeight = 1.0;
                    const double healthWeight = 1.5;
                    const double enemyUnitDamageWeight = 1.0;
                    const double enemyUnitCountWeight = 1.0;
                    const double productionTimeWeight = 0.5;

                    double bestValue = 0; 
                    const char* allUnitsTypes[] = { "K", "S", "A", "P", "R", "C", "W" };
                    char* bestType = nullptr;
                    int friendlyBaseId = baseUnit->id;
                    bool isGold = false;
                    //find any gold
                    for (int x = 0; x < map.getX(); ++x) {
                        for (int y = 0; y < map.getY(); ++y) {
                            if (map.getTile(y, x) == 6) {
                                isGold = true;
                            }
                        }
                    }
                    //prioritize 1 worker
                    if (isGold == true && isWorker == false && status.getGold() >= 100) {
                        orders.addOrder(friendlyBaseId, 'B', 'W');
                    }
                    //calculate best unit to produce
                    else {
                        for (int i = 0; i < 7; i++) {
                            if (getUnitParameterValue(allUnitsTypes[i][0], 3) <= status.getGold()) {
                                double costValue = costWeight / getUnitParameterValue(allUnitsTypes[i][0], 3);
                                double movementRangeValue = movementRangeWeight * (1.0 / static_cast<double>(getUnitParameterValue(allUnitsTypes[i][0], 2)));
                                double attackRangeValue = attackRangeWeight * (1.0 / static_cast<double>(getUnitParameterValue(allUnitsTypes[i][0], 4)));
                                double damageToOpponentBaseValue = damageToOpponentBaseWeight / getDamage(allUnitsTypes[i][0], 'B');
                                double healthValue = healthWeight * (1.0 / static_cast<double>(getUnitParameterValue(allUnitsTypes[i][0], 1)));
                                char enemyUnitType = '0';
                                //find enemy base
                                for (const auto& enemyUnit : allUnits) {
                                    if (enemyUnit->team == 'E' && enemyUnit->type == 'B') {
                                        if (Base* enemyBaseUnit = dynamic_cast<Base*>(enemyUnit)) {
                                            if (enemyBaseUnit->checkProduction() == true) {
                                                enemyUnitType = enemyBaseUnit->getProductionType(); //get enemy production to counter it
                                            }
                                        }
                                    }
                                }
                                //if enemy base is not producing anything
                                if (enemyUnitType != '0') {
                                    double enemyUnitDamageValue = enemyUnitDamageWeight / static_cast<double>(getDamage (allUnitsTypes[i][0], enemyUnitType));
                                }
                                else {
                                    double enemyUnitDamageValue = 0;
                                }
                                double productionTimeValue = productionTimeWeight * (1.0 / getUnitParameterValue(allUnitsTypes[i][0], 5));

                                //add all values
                                double totalValue = costValue + movementRangeValue + attackRangeValue + damageToOpponentBaseValue + healthValue + productionTimeValue;
                                if (bestValue < totalValue) {
                                    bestValue = totalValue; //remember best value
                                    bestType = const_cast<char*>(allUnitsTypes[i]); //remember best type
                                }
                            }
                        }
                        //add order to produce
                        if (bestType != nullptr) {
                            orders.addOrder(friendlyBaseId, 'B', bestType[0]);
                        }

                    }
                }
            }

        }
    }
}

int evaluateWorkerMove(const Unit& unit, const Map& map, Status& status,int newX, int newY) {
    bool isOnGold = false;
    int closestGoldDistance=1500; //some big range
    //find any gold
    for (int x = 0; x < map.getX(); ++x) {
        for (int y = 0; y < map.getY(); ++y) {
            if (map.getTile(y, x) == 6) {
                int distance = std::abs(x - newX) + std::abs(y - newY);
                closestGoldDistance = std::min(closestGoldDistance, distance);
                if (newX == x && newY == y) {
                    isOnGold = true;
                }
            }
        }
    }

    int goldDistanceVaue = std::max(map.getX(), map.getY()) - closestGoldDistance;
    int balancedGoldDistanceValue = std::min(goldDistanceVaue, 15);
    //check if in range of enemies attacks
    std::vector<Unit*> allUnits = status.getUnits();
    int canBeAttacked = 0;
    for (const auto& opponentUnit : allUnits) {
        if (opponentUnit->team == 'E' && opponentUnit->type != 'B' && std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - unit.y) <= opponentUnit->attackRange + opponentUnit->movementRange) {
            canBeAttacked++;
        }
    }

    int moreValue = 0;
    moreValue += isOnGold ? 49 : 0;
    moreValue -= canBeAttacked * 50;
    moreValue += balancedGoldDistanceValue;
    return moreValue;
}

int evaluateMove(const Unit& unit, const Map& map,Status& status, bool isAttacking, bool towardsEnemyBase, bool towardsFriendlyBase, int newX, int newY) {
   
    //check if in range of enemy attacks
    std::vector<Unit*> allUnits = status.getUnits();
    int canBeAttacked = 0;
    for (const auto& opponentUnit : allUnits) {
        if (opponentUnit->team=='E'&& opponentUnit->type!='B' && std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY) <= opponentUnit->attackRange+opponentUnit->movementRange) {
            canBeAttacked++;
        }
    }

    //check if any target in range
    bool canAttack = false;
    for (const auto& opponentUnit : allUnits) {
        if (isAttacking == true && opponentUnit->team == 'E' && opponentUnit->type != 'B' && std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY) <= unit.attackRange) {
            canAttack = true;
        }
    }


    //find closest gold distance
    int closestGoldDistance = map.getY() + map.getX(); 
    for (int x = 0; x < map.getX(); ++x) {
        for (int y = 0; y < map.getY(); ++y) {
            if (map.getTile(y, x) == 6) { 
                int distance = std::abs(x - newX) + std::abs(y - newY);
                closestGoldDistance = std::min(closestGoldDistance, distance);
            }
        }
    }

    int goldDistanceVaue = std::max(map.getX(), map.getY()) - closestGoldDistance;
    int balancedGoldDistanceValue = std::min(goldDistanceVaue, 15);


    bool canAttackOpponentBase = false;
    int i = 0;
    //find enemy base
    while (allUnits[i]->team != 'E' && allUnits[i]->type != 'B') {
        i++;
    }
    //check if can attack opponents base
    Unit* opponentBase = allUnits[i];
    for (const auto& friendlyUnit : allUnits) {
        if (unit.movementRange > 0 && isAttacking == true && std::abs(newX - opponentBase->x) + std::abs(newY - opponentBase->y) <= unit.attackRange) {
            canAttackOpponentBase = true;
            break;
        }
    }


    i = 0;
    //find own base
    while (allUnits[i]->team != 'P' && allUnits[i]->type != 'B') {
        i++;
    }
    //check if base is safe
    bool isBaseSafe = true;
    Unit* playerBase = allUnits[i];
    for (const auto& opponentUnit : allUnits) {
        if (opponentUnit->team=='E'&&opponentUnit->type!='B' && opponentUnit->movementRange > 0 && (std::abs(opponentUnit->x - playerBase->x + std::abs(opponentUnit->y - playerBase->y) <= opponentUnit->movementRange || std::abs(opponentUnit->x - playerBase->x) + std::abs(opponentUnit->y - playerBase->y) <= opponentUnit->attackRange+opponentUnit->movementRange))) {
            isBaseSafe = false;
            break;
        }
    }
    //check if can save base
    bool saveBase = false;
    if (isBaseSafe == false && towardsFriendlyBase == true) {
        saveBase = true;
    }


    //sum all values
    int moveValue = 0;
    moveValue += canAttack ? 60 : 0;
    moveValue -= canBeAttacked * 10;
    moveValue += balancedGoldDistanceValue;
    moveValue += canAttackOpponentBase ? 70 : 0;
    moveValue += saveBase ? 40 : 0;
    moveValue += towardsEnemyBase ? 15 : 0;

    return moveValue;
}