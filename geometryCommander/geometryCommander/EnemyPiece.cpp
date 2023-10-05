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

void EnemyPiece::MoveToCover()
{
	GameManager* gameManagerInstance = GameManager::GetInstance();
	Grid* grid = gameManagerInstance->GetGrid();
	GridBox* currentPosition = grid->GetBoxFromPosition(sf::Vector2i(xPos, yPos));
	std::map<coverDirection, int> tempMap = std::map<coverDirection, int>();
	bool notFlanked = true;
	
	for (unsigned int x = 0; x < grid->gridBoxes.size(); x++) {
		for (unsigned int y = 0; y < grid->gridBoxes[x].size(); y++) {
			//box is in move range
			if (MyUtils::GetInstance()->ManhattanDistance(currentPosition->xPos, currentPosition->yPos, grid->gridBoxes[x][y]->xPos, grid->gridBoxes[x][y]->yPos) <= moveRange) {
				//box can be moved to 
				if (grid->gridBoxes[x][y]->GetType() != occupied && grid->gridBoxes[x][y]->GetType() != fullCover) {
					tempMap = grid->GetCoverAtPosition(grid->gridBoxes[x][y]);

					if (tempMap.count(coverDirection::UP) != 0) {
						for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
						{
							//there is a piece behind the position being checked
							if (playerPiece->GetIndex().y > grid->gridBoxes[x][y]->index.y) {
								notFlanked = false;
								if (tempMap.count(coverDirection::RIGHT) != 0 && playerPiece->GetIndex().x > grid->gridBoxes[x][y]->index.x) {
									notFlanked = true;
								}
								else if (tempMap.count(coverDirection::LEFT) != 0 && playerPiece->GetIndex().x >= grid->gridBoxes[x][y]->index.x) {
									notFlanked = true;
								}

							}
						}
						if (notFlanked) {
							grid->MovePiece(currentPosition, grid->gridBoxes[x][y]);
							return;
						}
					}
					else if (tempMap.count(coverDirection::DOWN) != 0) {
						for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
						{
							//there is a piece above the position being checked
							if (playerPiece->GetIndex().y <= index.y) {
								notFlanked = false;
								if (tempMap.count(coverDirection::RIGHT) != 0 && playerPiece->GetIndex().x > grid->gridBoxes[x][y]->index.x) {
									notFlanked = true;
								}
								else if (tempMap.count(coverDirection::LEFT) != 0 && playerPiece->GetIndex().x >= grid->gridBoxes[x][y]->index.x) {
									notFlanked = true;
								}
							}
						}
						if (notFlanked) {
							grid->MovePiece(currentPosition, grid->gridBoxes[x][y]);
							return;
						}
					}
					else if (tempMap.count(coverDirection::LEFT) != 0) {
						for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
						{
							//there is a piece to the right of the position being checked
							if (playerPiece->GetIndex().x > index.x) {
								notFlanked = false;
								if (tempMap.count(coverDirection::UP) != 0 && playerPiece->GetIndex().y > grid->gridBoxes[x][y]->index.y) {
									notFlanked = true;
								}
								else if (tempMap.count(coverDirection::DOWN) != 0 && playerPiece->GetIndex().y <= grid->gridBoxes[x][y]->index.y) {
									notFlanked = true;
								}
							}
						}
						if (notFlanked) {
							grid->MovePiece(currentPosition, grid->gridBoxes[x][y]);
							return;
						}
					}
					else if (tempMap.count(coverDirection::RIGHT) != 0) {
						for (PlayerPiece* playerPiece : gameManagerInstance->GetPlayerPieces())
						{
							//there is a piece to the left of the position being checked
							if (playerPiece->GetIndex().x <= index.x) {
								notFlanked = false;
								if (tempMap.count(coverDirection::UP) != 0 && playerPiece->GetIndex().y > grid->gridBoxes[x][y]->index.y) {
									notFlanked = true;
								}
								else if (tempMap.count(coverDirection::DOWN) != 0 && playerPiece->GetIndex().y <= grid->gridBoxes[x][y]->index.y) {
									notFlanked = true;
								}
							}
						}
						if (notFlanked) {
							grid->MovePiece(currentPosition, grid->gridBoxes[x][y]);
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
	GameManager* gameManagerInstance = GameManager::GetInstance();
	//this is just a temp in the future this will impliment target selection
	Attack(gameManagerInstance->GetPlayerPieces()[0]);
}
