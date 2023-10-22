#include "EnemyPiece.h"
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "BehaviorTreeActionNode.h"
#include "BehaviorTreeConditionalNode.h"
#include "GameManager.h"
#include <iostream>
EnemyPiece::EnemyPiece(int _xPos, int _yPos) : GamePiece(_xPos, _yPos)
{
	radius = 20;
	visual = new sf::CircleShape(radius);
	visual->setOrigin(radius , radius);
	visual->setPosition(sf::Vector2f(xPos, yPos));
	visual->setFillColor(sf::Color(255, 0, 0));
	ConstructBehaviorTree();
}

EnemyPiece::~EnemyPiece()
{
	delete visual;
	if (flankPosition != nullptr) {
		delete flankPosition;
	}
	//bugged on acc of multiple nodes having the same child, need to figure out if there is a way to keep them all being deleted at once or just use a copy constuctor to create several identical nodes
	//delete BehaviorTreeRoot;

	for (BehaviorTreeNode* node : BehaviorTreeDeleteList) {
		delete node;
	}
}

void EnemyPiece::Draw(sf::RenderWindow* window)
{
	visual->setPosition(xPos, yPos);
	window->draw(*visual);
	//draws projecile, will update in the future to clean
	GamePiece::Draw(window);
	
}

void EnemyPiece::TakeTurn()
{
	turnTaken = true;
	BehaviorTreeRoot->Traverse();
}

void EnemyPiece::ConstructBehaviorTree()
{
	BehaviorTreeActionNode<EnemyPiece>* Advance = new BehaviorTreeActionNode<EnemyPiece>(this, &EnemyPiece::Advance);
	BehaviorTreeActionNode<EnemyPiece>* Attack = new BehaviorTreeActionNode<EnemyPiece>(this, &EnemyPiece::AttackPlayerPiece);
	BehaviorTreeConditionalNode<EnemyPiece>* CheckTargetInRange = new BehaviorTreeConditionalNode<EnemyPiece>(this, Attack, Advance, &EnemyPiece::AnyTargetInRange);
	BehaviorTreeActionNode<EnemyPiece>* MoveToFlank = new BehaviorTreeActionNode<EnemyPiece>(this, &EnemyPiece::MoveToFlank);
	BehaviorTreeConditionalNode<EnemyPiece>* CheckFlankInRange = new BehaviorTreeConditionalNode<EnemyPiece>(this, MoveToFlank, CheckTargetInRange, &EnemyPiece::FlankInRange);
	BehaviorTreeConditionalNode<EnemyPiece>* CheckTargetOutOfCover = new BehaviorTreeConditionalNode<EnemyPiece>(this, Attack, CheckFlankInRange, &EnemyPiece::AnyTargetOutOfCover);
	BehaviorTreeConditionalNode<EnemyPiece>* CheckTargetPointBlank = new BehaviorTreeConditionalNode<EnemyPiece>(this, Attack, CheckTargetOutOfCover, &EnemyPiece::AnyTargetPointBlank);
	BehaviorTreeActionNode<EnemyPiece>* MoveToCover = new BehaviorTreeActionNode<EnemyPiece>(this, &EnemyPiece::MoveToCover);
	BehaviorTreeConditionalNode<EnemyPiece>* CheckThisFlanked = new BehaviorTreeConditionalNode<EnemyPiece>(this, MoveToCover, CheckTargetPointBlank, &EnemyPiece::IsFlanked);
	BehaviorTreeConditionalNode<EnemyPiece>* CheckThisInCover = new BehaviorTreeConditionalNode<EnemyPiece>(this, CheckThisFlanked, MoveToCover, &EnemyPiece::IsInCover);
	BehaviorTreeRoot = new BehaviorTreeNode(CheckThisInCover, nullptr);

	//uses a list to delete all behavior nodes so that multiple conditions can lead to the same behavior node without needing to copy or creating an error where nodes are deleted twice
	BehaviorTreeDeleteList.push_back(Advance);
	BehaviorTreeDeleteList.push_back(Attack);
	BehaviorTreeDeleteList.push_back(CheckTargetInRange);
	BehaviorTreeDeleteList.push_back(MoveToFlank);
	BehaviorTreeDeleteList.push_back(CheckFlankInRange);
	BehaviorTreeDeleteList.push_back(CheckTargetOutOfCover);
	BehaviorTreeDeleteList.push_back(CheckTargetPointBlank);
	BehaviorTreeDeleteList.push_back(MoveToCover);
	BehaviorTreeDeleteList.push_back(CheckThisFlanked);
	BehaviorTreeDeleteList.push_back(CheckThisInCover);
	BehaviorTreeDeleteList.push_back(BehaviorTreeRoot);
}

bool EnemyPiece::IsInCover()
{
	return !coverMap.empty();
}

bool EnemyPiece::IsFlanked()
{
	return isPositionFlanked(GameManager::GetInstance()->GetGrid()->GetBoxFromOccupyingPiece(this));
}

bool EnemyPiece::AnyTargetOutOfCover()
{
	return TargetOutOfCover() != nullptr;
}

bool EnemyPiece::FlankInRange()
{
	//can't store manager instance as a class field because it will create a cyclical reference, so needs to get it in every scope where I need it
	GameManager* gameManagerInstance = GameManager::GetInstance();
	Grid* grid = gameManagerInstance->GetGrid();
	for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces()) {
		for (unsigned int x = 0; x < grid->GetWidth(); x++) {
			for (unsigned int y = 0; y < grid->GetHeight(); y++) {
				if (MyUtils::GetInstance()->ManhattanDistance(index.x, index.y, x, y) <= moveRange) {
					if (!grid->GetCoverAtPosition(grid->gridBoxes[x][y]).empty()) {
						if (playerPiece->isPositionFlanked(sf::Vector2f(x, y)) && !this->isPositionFlanked(grid->gridBoxes[x][y])) {
							flankPosition = new sf::Vector2f(x, y);
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool EnemyPiece::AnyTargetInRange()
{
	
	for (PlayerPiece* playerPiece : GameManager::GetInstance()->GetPlayerPieces()) {
		//if even one target is in range returns true
		if (TargetInRange(playerPiece)) {
			return true;
		}
	}
	//if all targets are out of range returns false
	return false;
}

bool EnemyPiece::AnyTargetPointBlank()
{
	return TargetPointBlank() != nullptr;
}

void EnemyPiece::MoveToCover()
{
	GameManager* gameManagerInstance = GameManager::GetInstance();
	Grid* grid = gameManagerInstance->GetGrid();
	std::map<coverDirection, int> tempMap = std::map<coverDirection, int>();
	
	
	for (unsigned int x = 0; x < grid->gridBoxes.size(); x++) {
		for (unsigned int y = 0; y < grid->gridBoxes[x].size(); y++) {
			bool notFlanked = true;
			//box is in move range
			if (MyUtils::GetInstance()->ManhattanDistance(GetIndex().x, GetIndex().y, grid->gridBoxes[x][y]->index.x, grid->gridBoxes[x][y]->index.y) <= moveRange) {
				//box can be moved to 
				if (grid->gridBoxes[x][y]->GetType() == empty) {
					std::map<coverDirection, int> tempMap = grid->GetCoverAtPosition(grid->gridBoxes[x][y]);
					if (!tempMap.empty()) {
						if (!isPositionFlanked(grid->gridBoxes[x][y])) {
							grid->MovePiece(grid->GetBoxFromOccupyingPiece(this), grid->gridBoxes[x][y]);
							return;
						}
					}
					
				}
			}
		}
	}
	//no unoccupied cover positions in range
	AttackPlayerPiece();
}

void EnemyPiece::AttackPlayerPiece()
{
	if (AnyTargetInRange()) {
		Attack(SelectTarget());
	}
	
}

void EnemyPiece::MoveToFlank()
{
	if (flankPosition != nullptr) {
		Grid* grid = GameManager::GetInstance()->GetGrid();
		grid->MovePiece(grid->GetBoxFromOccupyingPiece(this), grid->gridBoxes[flankPosition->x][flankPosition->y]);
	}
	else {
		std::cout << "ERROR WITH FLANK POSITION DECLARATION";
	}
}

void EnemyPiece::Advance()
{
	int shortestDistanceToTarget = INT16_MAX;
	PlayerPiece* target = nullptr;
	GameManager* gameManagerInstance = GameManager::GetInstance();
	Grid* grid = gameManagerInstance->GetGrid();
	for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces()) {
		int distance = MyUtils::GetInstance()->ManhattanDistance(grid->gridBoxes[index.x][index.y], grid->gridBoxes[playerPiece->GetIndex().x][playerPiece->GetIndex().y]);
		if (distance < shortestDistanceToTarget) {
			shortestDistanceToTarget = distance;
			target = playerPiece;
		}
	}
	sf::Vector2f indexCache;
	for (unsigned int x = 0; x < grid->gridBoxes.size(); x++) {
		for (unsigned int y = 0; y < grid->gridBoxes[x].size(); y++) {
			int distanceToTarget = MyUtils::GetInstance()->ManhattanDistance(grid->gridBoxes[x][y], grid->gridBoxes[target->GetIndex().x][target->GetIndex().y]);
			if (distanceToTarget < shortestDistanceToTarget) {
				int distanceFromPosition = MyUtils::GetInstance()->ManhattanDistance(grid->gridBoxes[x][y], grid->gridBoxes[index.x][index.y]);
				if (distanceFromPosition < moveRange) {
					if (!isPositionFlanked(grid->gridBoxes[index.x][index.y])) {
						grid->MovePiece(grid->gridBoxes[index.x][index.y], grid->gridBoxes[x][y]);
						return;
					}
					else {
						//stores a position that is both closer and in range but may not be in cover or flanked for if there are no positions where !isPositionFlanked is true
						indexCache = sf::Vector2f(x, y);
					}
				}
			}
			
		}
	}

	grid->MovePiece(grid->gridBoxes[index.x][index.y], grid->gridBoxes[indexCache.x][indexCache.y]);
}

GamePiece* EnemyPiece::SelectTarget()
{
	GameManager* gameManagerInstance = GameManager::GetInstance();
	Grid* grid = gameManagerInstance->GetGrid();
	//GamePiece* currentTarget;
	//this is just a temp in the future this will impliment target selection
	for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces()) {
		int distance = MyUtils::GetInstance()->ManhattanDistance(grid->GetBoxFromOccupyingPiece(this), grid->GetBoxFromOccupyingPiece(playerPiece));
		if (distance <= attackRange) {
			//target is within point blank range
			if (distance <= 2) {
				return playerPiece;
			}
			//target is out of cover
			else if (playerPiece->coverMap.empty()) {
				return playerPiece;
			}
			//target is low health
			else if (playerPiece->GetHealth() <= maxDamage) {
				return playerPiece;
			}
			//checks induvidual flank conditions
			else {
				if (index.y <= playerPiece->GetIndex().y && playerPiece->coverMap.count(UP) == 0) {
					if (playerPiece->coverMap.count(RIGHT) == 0 && playerPiece->coverMap.count(LEFT) == 0) {
						return playerPiece;
					}
					else {
						if (playerPiece->coverMap.count(RIGHT) != 0) {
							if (index.x >= playerPiece->GetIndex().x) {
								return playerPiece;
							}
						}
						else if (playerPiece->coverMap.count(LEFT) != 0) {
							if (index.x <= playerPiece->GetIndex().x) {
								return playerPiece;
							}
						}
					}
				}
				else if (index.y >= playerPiece->GetIndex().y && playerPiece->coverMap.count(DOWN) == 0) {
					if (playerPiece->coverMap.count(RIGHT) == 0 && playerPiece->coverMap.count(LEFT) == 0) {
						return playerPiece;
					}
					else {
						if (playerPiece->coverMap.count(RIGHT) != 0) {
							if (index.x >= playerPiece->GetIndex().x) {
								return playerPiece;
							}
						}
						else if (playerPiece->coverMap.count(LEFT) != 0) {
							if (index.x <= playerPiece->GetIndex().x) {
								return playerPiece;
							}
						}
					}
				}
				else if (index.x >= playerPiece->GetIndex().x && playerPiece->coverMap.count(RIGHT) == 0) {
					if (playerPiece->coverMap.count(UP) == 0 && playerPiece->coverMap.count(DOWN) == 0) {
						return playerPiece;
					}
					else {
						if (playerPiece->coverMap.count(DOWN) != 0) {
							if (index.y <= playerPiece->GetIndex().y) {
								return playerPiece;
							}
						}
						else if (playerPiece->coverMap.count(UP) != 0) {
							if (index.y >= playerPiece->GetIndex().y) {
								return playerPiece;
							}
						}
					}
				}
				else if (index.x <= playerPiece->GetIndex().x && playerPiece->coverMap.count(LEFT) == 0) {
					if (playerPiece->coverMap.count(DOWN) != 0) {
						if (index.y <= playerPiece->GetIndex().y) {
							return playerPiece;
						}
					}
					else if (playerPiece->coverMap.count(UP) != 0) {
						if (index.y >= playerPiece->GetIndex().y) {
							return playerPiece;
						}
					}
				}
			}
		}
	}
	//if there are no special targeting conditions found returns the first player piece in range
	for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces()) {
		int distance = MyUtils::GetInstance()->ManhattanDistance(grid->GetBoxFromOccupyingPiece(this), grid->GetBoxFromOccupyingPiece(playerPiece));
		if (distance <= attackRange) {
			return playerPiece;
		}
	}
	//error case, should never be returned since the method call checks if there are any targets in range before hand
	return nullptr;
}

bool EnemyPiece::TargetInRange(GamePiece* playerPiece)
{
	Grid* grid = GameManager::GetInstance()->GetGrid();
	return (MyUtils::GetInstance()->ManhattanDistance(grid->GetBoxFromOccupyingPiece(this), grid->GetBoxFromOccupyingPiece(playerPiece)) <= attackRange);
}
//possibly delete in the future since GamePiece defines this, not sure how this will work with function pointer so I am keeping for now
bool EnemyPiece::isPositionFlanked(GridBox* position)
{

	GameManager* gameManagerInstance = GameManager::GetInstance();
	bool flanked = false;
	Grid* grid = gameManagerInstance->GetGrid();
	std::map<coverDirection, int> tempMap = grid->GetCoverAtPosition(position);
	
	if (tempMap.count(coverDirection::UP) != 0) {
		for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
		{
			//there is a piece behind the position being checked
			if (playerPiece->GetIndex().y >= position->index.y) {
				flanked = true;
				//checks if the prospective position has cover from multiple angles making the flank not apply
				if (tempMap.count(coverDirection::RIGHT) != 0 && playerPiece->GetIndex().x > position->index.x) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::LEFT) != 0 && playerPiece->GetIndex().x < position->index.x) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
	else if (tempMap.count(coverDirection::DOWN) != 0) {
		for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
		{
			//there is a piece above the position being checked
			if (playerPiece->GetIndex().y <= position->index.y) {
				flanked = true;
				if (tempMap.count(coverDirection::RIGHT) != 0 && playerPiece->GetIndex().x > position->index.x) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::LEFT) != 0 && playerPiece->GetIndex().x < position->index.x) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
	else if (tempMap.count(coverDirection::LEFT) != 0) {
		for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
		{
			//there is a piece to the right of the position being checked
			if (playerPiece->GetIndex().x >= position->index.x) {
				flanked = true;
				if (tempMap.count(coverDirection::UP) != 0 && playerPiece->GetIndex().y > position->index.y) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::DOWN) != 0 && playerPiece->GetIndex().y < position->index.y) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
	else if (tempMap.count(coverDirection::RIGHT) != 0) {
		for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
		{
			//there is a piece to the left of the position being checked
			if (playerPiece->GetIndex().x <= position->index.x) {
				flanked = true;
				if (tempMap.count(coverDirection::UP) != 0 && playerPiece->GetIndex().y > position->index.y) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::DOWN) != 0 && playerPiece->GetIndex().y < position->index.y) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
}

GamePiece* EnemyPiece::TargetOutOfCover()
{
	for (PlayerPiece* playerPiece : GameManager::GetInstance()->GetPlayerPieces()) {
		if (playerPiece->coverMap.empty()) {
			return playerPiece;
		}
	}

	return nullptr;
}

GamePiece* EnemyPiece::TargetPointBlank()
{
	for (PlayerPiece* potentialTarget : GameManager::GetInstance()->GetPlayerPieces()) {
		if (MyUtils::GetInstance()->ManhattanDistance(potentialTarget->GetIndex().x, potentialTarget->GetIndex().y, index.x, index.y) <= 2) {
			return potentialTarget;
		}
	}
	return nullptr;
}
