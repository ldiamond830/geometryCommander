#pragma once
#include "GamePiece.h"
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "BehaviorTreeNode.h"
#include "MyUtils.h"
class EnemyPiece : public GamePiece
{
public:
	EnemyPiece(int xPos, int yPos);
	~EnemyPiece();
	void Draw(sf::RenderWindow* window) override;
	void TakeTurn();
	//reset to protected after testing
	bool FlankInRange();
protected:
	sf::CircleShape* visual;
	sf::Vector2f* flankPosition;
	BehaviorTreeNode* BehaviorTreeRoot;
	virtual void ConstructBehaviorTree();
	//B Tree condition methods
	bool IsInCover();
	bool IsFlanked();
	
	bool AnyTargetInRange();
	//B Tree Behavior methods
	void MoveToCover();
	void AttackPlayerPiece();
	void MoveToFlank();
	void Advance();
	//helper methods
	GamePiece* SelectTarget();
	bool TargetInRange(GamePiece*);
	bool isPositionFlanked(GridBox*);
};

