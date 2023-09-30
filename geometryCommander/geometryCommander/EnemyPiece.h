#pragma once
#include "GamePiece.h"
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "BehaviorTreeNode.h"
class EnemyPiece : public GamePiece
{
public:
	EnemyPiece(int xPos, int yPos);
	~EnemyPiece();
	void Draw(sf::RenderWindow* window) override;
	void TakeTurn();
	virtual void ConstructBehaviorTree();
protected:
	sf::CircleShape* visual;
	float radius = 10;
	BehaviorTreeNode* BehaviorTreeRoot;
	//B Tree condition methods
	bool IsInCover();
	//B Tree Behavior methods
	void MoveToCover();
	void Attack();
};

