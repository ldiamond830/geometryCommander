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
	moveRange = 3;

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
	BehaviorTreeConditionalNode<EnemyPiece>* n1 = new BehaviorTreeConditionalNode<EnemyPiece>(this, test1, test2, &EnemyPiece::IsInCover);
	BehaviorTreeRoot = new BehaviorTreeNode(n1, nullptr);
}

bool EnemyPiece::IsInCover()
{
	return !coverMap.empty();
}

void EnemyPiece::MoveToCover()
{
}

void EnemyPiece::AttackPlayerPiece()
{
	GameManager* instance = GameManager::GetInstance();
	Attack(instance->GetPlayerPieces()[0]);
}
