#include "EnemyPiece.h"
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "BehaviorTreeActionNode.h"
#include "BehaviorTreeConditionalNode.h"
#include "GameManager.h"
EnemyPiece::EnemyPiece(int _xPos, int _yPos)
{
	xPos = _xPos;
	yPos = _yPos;
	health = 10;
	maxDamage = 3;
	minDamage = 1;
	moveRange = 5;
	attackRange = 7;
	radius = 20;
	visual = new sf::CircleShape(radius);
	visual->setOrigin(radius , radius);
	visual->setPosition(sf::Vector2f(xPos, yPos));
	visual->setFillColor(sf::Color(255, 0, 0));
	ConstructBehaviorTree();
	//visual->setOutlineThickness(5.0f);
}

EnemyPiece::~EnemyPiece()
{
	delete visual;
	//delete BehaviorTreeRoot;
}

void EnemyPiece::Draw(sf::RenderWindow* window)
{
	visual->setPosition(xPos, yPos);
	window->draw(*visual);
}

void EnemyPiece::TakeTurn()
{
	turnTaken = true;
	BehaviorTreeRoot->Traverse();
}

void EnemyPiece::ConstructBehaviorTree()
{
	BehaviorTreeActionNode<EnemyPiece>* test1 = new BehaviorTreeActionNode<EnemyPiece>(this, &EnemyPiece::MoveToCover);
	BehaviorTreeActionNode<EnemyPiece>* test2 = new BehaviorTreeActionNode<EnemyPiece>(this, &EnemyPiece::AttackPlayerPiece);;
	BehaviorTreeConditionalNode<EnemyPiece>* n1 = new BehaviorTreeConditionalNode<EnemyPiece>(this, test2, test1, &EnemyPiece::IsInCover);
	BehaviorTreeRoot = new BehaviorTreeNode(n1, nullptr);
}

bool EnemyPiece::IsInCover()
{
	return !coverMap.empty();
}

bool EnemyPiece::IsFlanked()
{
	return isPositionFlanked(GameManager::GetInstance()->GetGrid()->GetBoxFromOccupyingPiece(this));
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
