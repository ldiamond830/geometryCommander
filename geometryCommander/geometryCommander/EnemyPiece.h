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
	void Advance();
protected:
	sf::CircleShape* visual;
	sf::Vector2f* flankPosition;
	BehaviorTreeNode* BehaviorTreeRoot;
	std::vector<BehaviorTreeNode*> BehaviorTreeDeleteList;
	virtual void ConstructBehaviorTree();
	//B Tree condition methods
	bool IsInCover();
	bool IsFlanked();
	bool AnyTargetOutOfCover();
	bool AnyTargetInRange();
	bool AnyTargetPointBlank();
	//B Tree Behavior methods
	void MoveToCover();
	void AttackPlayerPiece();
	void MoveToFlank();
	
	//helper methods
	GamePiece* SelectTarget();
	bool TargetInRange(GamePiece*);
	bool isPositionFlanked(GridBox*);
	GamePiece* TargetOutOfCover();
	GamePiece* TargetPointBlank();
};

