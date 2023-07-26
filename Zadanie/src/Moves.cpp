#include <vector>
#include <cmath>
#include <iostream>
#include "Unit.h"
#include "Map.h"
#include "Moves.h"
#include "Status.h"
#include "MapOfUnits.h"
#include "Orders.h"
#include "Base.h"

int getUnitParameterValue(const char type,int parameter){
    const std::string parametersTable[7][6] = {  //tab containing statistics of units
        // type         health   movementRange  cost     attack range  producing time
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
            //std::cout<<"unit xy, newxy, mapxy: "<< unit.x << " "<< unit.y << " "<<newX<<" "<<newY<<" "<<map.getX()<<" " <<map.getY()<<std::endl;
            //check if new tile is in range
            if ((std::abs(newX - unit.x) + std::abs(newY - unit.y)) <= unit.movementRange) {
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
    int bestEval =-100;             //evaluation of best move
    auto allMoves = getPossibleMoves(unit, map, mou); //all possible moves from function
    for (std::pair<int, int> move : allMoves) {
        //if worker
        if (unit.type == 'W') {
            //use worker function to evaluate move value
            int eval = evaluateWorkerMove(unit, map, status, move.second,move.first);
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
            int distanceToOpponentBaseBefore = (std::abs(allUnits[i]->x - unit.x) + std::abs(allUnits[i]->y) - unit.y);
            int distanceToOpponentBaseAfter = (std::abs(allUnits[i]->x - move.second) + std::abs(allUnits[i]->y) - move.first);
            if (distanceToOpponentBaseAfter < distanceToOpponentBaseBefore) {
                towardsEnemyBase = true;
                
            }
            //use function to evaluate move value
            int eval = evaluateMove(unit, map, status, isAttacking,towardsEnemyBase,!towardsEnemyBase,move.second,move.first );
            //std::cout<<"Move: "<< unit.x <<" " << unit.y<<" " << move.second << " " <<move.first<< " " <<eval;
            //save best evaluation and move
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }
    //if any move possible, order best, update status, update map of units
    if (allMoves.size() != 0) {
        //std::cout<<"BestMove: "<<unit.id<<", " <<unit.x <<" " << unit.y<<" " << bestMove.second << " " <<bestMove.first<< " " <<bestEval;
        orders.addOrder(unit.id, 'M', bestMove.second, bestMove.first);
        status.changeLocation(unit.id, bestMove.second, bestMove.first);
        mou.changeLocation(unit.x, unit.y, bestMove.second, bestMove.first);
    }
}

void findBestAttacks(const Map& map, MapOfUnits& mou, Status& status, Orders& orders){
    std::vector<Unit*> allUnits = status.getUnits(); //get all units
    for (const auto& friendlyUnit : allUnits) {
        //check all friendly units that are not base
        if (friendlyUnit->team == 'P' && friendlyUnit->type !='B' && friendlyUnit->movementRange>0) {
            //find all possible targets
            std::vector<Unit*> possibleTargets;
            for (const auto& opponentUnit : allUnits) {
                //check if enemies are in range
                if (opponentUnit->team == 'E' && opponentUnit->health>0 && (std::abs(opponentUnit->x - friendlyUnit->x) + std::abs(opponentUnit->y - friendlyUnit->y)) <= friendlyUnit->attackRange) {
                   // std::cout<<"Opponent added: "<<opponentUnit->id<<", "<<opponentUnit->x<<", "<<opponentUnit->y<<", "<<friendlyUnit->id<<", "<<friendlyUnit->x<<", "<<friendlyUnit->y<<std::endl;
                    possibleTargets.push_back(opponentUnit); //add possible target
                }
            }
            int iterator=0;
            int mostDamage = 0;
            int bestTargetId=0;
            int closestId=0;
            std::vector<int> bestTargets;
            //if any possible target, find best based on damage dealt
            if (possibleTargets.size() > 0) {
                for (int i = 0; i < possibleTargets.size(); i++) {
              	    if (possibleTargets[i]->health<=getDamage(friendlyUnit->type, possibleTargets[i]->type) && possibleTargets[i]->type == 'B'){
              	             orders.addOrder(friendlyUnit->id, 'A', possibleTargets[i]->id);
                	     status.getDamage(bestTargetId, getDamage(friendlyUnit->type, possibleTargets[i]->type)); 
                	     return;
              	    }
		    else if (possibleTargets[i]->health>0 && (getDamage(friendlyUnit->type, possibleTargets[i]->type)) > mostDamage) {
		    	    bestTargets.clear();
			    mostDamage = getDamage(friendlyUnit->type, possibleTargets[i]->type);
			    bestTargetId = possibleTargets[i]->id;
			    bestTargets.push_back(i);
		    }
		    else if(possibleTargets[i]->health>0 && (getDamage(friendlyUnit->type, possibleTargets[i]->type)) == mostDamage) {
		    	 bestTargets.push_back(i);
		    }
		 }
		 
		  for (int i = 0; i < bestTargets.size(); i++) {
		  	int closestDistance=1110;
		  	if(std::abs(possibleTargets[bestTargets[i]]->x - friendlyUnit->x) + std::abs(possibleTargets[bestTargets[i]]->y - friendlyUnit->y)<=closestDistance){
		  		closestDistance=std::abs(possibleTargets[bestTargets[i]]->x - friendlyUnit->x) + std::abs(possibleTargets[bestTargets[i]]->y - friendlyUnit->y);
		  		closestId=bestTargets[i];
		  		if(possibleTargets[bestTargets[i]]->health<=(getDamage(friendlyUnit->type, possibleTargets[bestTargets[i]]->type))){
		  			orders.addOrder(friendlyUnit->id, 'A', possibleTargets[closestId]->id);
                			status.getDamage(possibleTargets[closestId]->id, mostDamage);
		  		}
		  	}
		  }
		    
                
                //make best move, and update damage
		orders.addOrder(friendlyUnit->id, 'A', possibleTargets[closestId]->id);
                status.getDamage(possibleTargets[closestId]->id, mostDamage);	
             }

          }
      } 
   }

void findBestPurchase(const Map& map, MapOfUnits& mou, Status& status, Orders& orders){
    std::vector<Unit*> allUnits = status.getUnits();
    Base* myBase;
    bool isWorker = false;
    int workerAmount =0;
    int enemyWorkers=0;
    int enemyFightingUnits=0;
    int fightingUnitAmount=0;
    int knightsAmount=0;  //amount of enemy knights
    int swordsmanAmount=0;  //amount of friendly swordsman
    int catapultAmount=0;
    int friendlyKnightAmount=0;
    //find any worker
    for (const auto& friendlyUnit : allUnits) {
        if (friendlyUnit->team == 'P' && friendlyUnit->type == 'W') {
            isWorker = true;
            workerAmount++;
        }
        else if(friendlyUnit->team == 'P' && friendlyUnit->type != 'W' && friendlyUnit->type != 'B'){
        	fightingUnitAmount++;
        	if(friendlyUnit->type=='S'){
        		swordsmanAmount++;
        	}
        	else if(friendlyUnit->type=='K'){
        		friendlyKnightAmount++;
        	}
        } 
        else if(friendlyUnit->team == 'E' && friendlyUnit->type != 'W' && friendlyUnit->type != 'B'){
        	enemyFightingUnits++;
        	if(friendlyUnit->type=='K'){
        		knightsAmount++;
        	}
        	else if(friendlyUnit->type=='C'){
        		catapultAmount++;
        	}
        }
        else if(friendlyUnit->team == 'E' && friendlyUnit->type == 'W'){
        	enemyWorkers++;
        }
    }
    	for (const auto& friendlyUnit : allUnits){
        //find base
        if (friendlyUnit->team == 'P' && friendlyUnit->type == 'B') {
            if (Base* baseUnit = dynamic_cast<Base*>(friendlyUnit)) {
                if (baseUnit->checkProduction() == false) {

                    double bestValue = 0; 
                    const char* allUnitsTypes[] = { "K", "S", "A", "P", "R", "C", "W" };
                    char* bestType = nullptr;
                    int friendlyBaseId = baseUnit->id;
                    bool isGold = false;
                    int goldAmount=0;
                    //find any gold
                    for (int x = 0; x < map.getX(); ++x) {
                        for (int y = 0; y < map.getY(); ++y) {
                            if (map.getTile(y, x) == 6) {
                                isGold = true;
                                goldAmount++;
                            }
                        }
                    }
                    //prioritize 1 worker
                    if (isGold == true && isWorker == false && status.getGold() >= 100) {
                        orders.addOrder(friendlyBaseId, 'B', 'W');
                    }
                    //prioritize worker if there are more mines
                    else if(goldAmount>workerAmount*2&&fightingUnitAmount+1>=enemyFightingUnits&&status.getGold() >= 100){
                    	orders.addOrder(friendlyBaseId, 'B', 'W');
                    }
                    //prioritize catapults, archer, swordsman
                    else if(status.getGold() >= 800){
                    	bestType = const_cast<char*>(allUnitsTypes[5]);
                    }
                    //prioritize swodsman if has nights
                    else if(knightsAmount>swordsmanAmount && status.getGold() >= 250){
                    	bestType = const_cast<char*>(allUnitsTypes[1]);
                    }
                    else if(status.getGold() >= 250) {
                    	bestType = const_cast<char*>(allUnitsTypes[3]);
                    }
                  
                    if (bestType != nullptr) {
                            //std::cout<<"Base: "<< bestType[0]<<" Gold: " <<status.getGold()<< " Curr production"<< baseUnit->checkProduction()<<std::endl;
                            orders.addOrder(friendlyBaseId, 'B', bestType[0]);
                    }
                }
            }

        }
    }
    }


int evaluateWorkerMove(const Unit& unit, const Map& map, Status& status,int newX, int newY) {
    bool isOnGold = false;
    bool alreadyMined = false;
    int closestGoldDistance=1500; //some big range
    std::vector<Unit*> allUnits = status.getUnits();
    //find any gold
    for (int x = 0; x < map.getX(); ++x) {
        for (int y = 0; y < map.getY(); ++y) {
            if (map.getTile(y, x) == 6) {
            	for (const auto& opponentUnit : allUnits){
            		if (opponentUnit->type == 'W'){
            			alreadyMined=true;
            		}
            	}
            	if(alreadyMined==false){
		        int distance = std::abs(x - newX) + std::abs(y - newY);
		        closestGoldDistance = std::min(closestGoldDistance, distance);
		        if (newX == x && newY == y) {
		            isOnGold = true;
		        }
                }
            }
            alreadyMined=false;
        }
    }

    int goldDistanceVaue = std::max(map.getX(), map.getY()) - closestGoldDistance;
    int balancedGoldDistanceValue = std::min(goldDistanceVaue, 15);
    //check if in range of enemies attacks
    int canBeAttacked = 0;
    for (const auto& opponentUnit : allUnits) {
        if (opponentUnit->team == 'E' && opponentUnit->type != 'B' && std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY) <= opponentUnit->attackRange + opponentUnit->movementRange) {
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
    bool canWin=false;
    for (const auto& opponentUnit : allUnits) {
        if (opponentUnit->team=='E'&& opponentUnit->type!='B' && std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY) <= opponentUnit->attackRange+opponentUnit->movementRange) {
            canBeAttacked++;
        }
    }

    //check if any target in range
    bool canAttack = false;
    float inRangeBonus=0; //if on max range - get bonus, else penalty)
    for (const auto& opponentUnit : allUnits) {
        if (isAttacking == true && opponentUnit->team == 'E' && opponentUnit->type != 'B' && std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY) <= unit.attackRange) {
            canAttack = true;
            inRangeBonus=((std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY)))/unit.attackRange;
            inRangeBonus=inRangeBonus*50;
            inRangeBonus=std::round(inRangeBonus);
        }
    }
    
    //find closest enemy
    int closestEnemyDistance=10000;
    Unit* closestEnemy;
    bool towardsClosestEnemy = false;
    if (canAttack==false){
    	for (const auto& opponentUnit : allUnits) {
    		int enemyDistance;
    		if (opponentUnit->team=='E') {
    			int enemyDistance = std::abs(opponentUnit->x - newX) + std::abs(opponentUnit->y - newY);
    			if(closestEnemyDistance>enemyDistance){
    				closestEnemyDistance=enemyDistance;
    				closestEnemy = opponentUnit;
    			}
        	}
    	}
    	//check if moving towards closest enemy
    	int distanceToOpponentBefore = std::abs(unit.x - closestEnemy->x) + std::abs(unit.y - closestEnemy->y);
        int distanceToOpponentAfter = std::abs(newX - closestEnemy->x) + std::abs(newY - closestEnemy->y);
        if (distanceToOpponentAfter < distanceToOpponentBefore) {
                towardsClosestEnemy=true;
                
        }
    }
    
    //check if all move points were used
    bool maxDistance = false;
    if(std::abs(unit.x - newX) + std::abs(unit.y - newY)==getUnitParameterValue(unit.type,2)){
    	maxDistance = true;
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
    for(int j=0;j<allUnits.size();j++){
    	if(allUnits[j]->team == 'E' && allUnits[j]->type == 'B'){
    		i=j;
    		break;
    	}
    }
    //check if can attack opponents base
    Unit* opponentBase = allUnits[i];
        if (unit.movementRange > 0 && isAttacking == true && std::abs(newX - opponentBase->x) + std::abs(newY - opponentBase->y) <= unit.attackRange) {
            canAttackOpponentBase = true;
            if(opponentBase->health<=getDamage(unit.type, opponentBase->type)){
            	canWin=true;
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
        if (opponentUnit->team=='E'&&opponentUnit->type!='B'){
        	if(std::abs(opponentUnit->x - playerBase->x + std::abs(opponentUnit->y - playerBase->y) <= opponentUnit->movementRange || std::abs(opponentUnit->x - playerBase->x) + std::abs(opponentUnit->y - playerBase->y) < opponentUnit->attackRange+opponentUnit->movementRange)){
		    isBaseSafe = false;
		    break;
        	}
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
    moveValue += inRangeBonus*5;
    moveValue += balancedGoldDistanceValue;
    moveValue += towardsClosestEnemy ? 20: 0;
    moveValue += canAttackOpponentBase ? 70 : 0;
    moveValue += saveBase ? 40 : 0;
    moveValue += towardsEnemyBase ? 15 : 0;
    moveValue += canWin ? 10000 : 0;
    moveValue += maxDistance ? 10 : 0;
    /*std::cout << "Move: " << unit.x << " " << unit.y << " " << newX << " " << newY
          << " Warunki " << canAttack << ", " << canBeAttacked << ", " << inRangeBonus << ", "
          << balancedGoldDistanceValue << ", " << towardsClosestEnemy <<", "<<canAttackOpponentBase <<", " << saveBase << ", "
          << towardsEnemyBase << ", " << maxDistance << ", " << moveValue << std::endl;
    std::cout<<newX <<", "<< opponentBase->x <<", "<<newY <<", "<< opponentBase->y<<", " << unit.attackRange << std::endl;*/
    return moveValue;
}
