#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stack>
#include <map>
enum coverDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class GamePiece
{
protected:
	float xPos;
	float yPos;
	int health;
	int moveRange;
	int maxDamage;
	int minDamage;
	
	bool atDestination = false;
	int CalcDamage();
	bool ChanceToHit(GamePiece* target);
	sf::Vector2f index;
	int accuracy = 75;
	//each child class will need to define visual since SMFL doesn't seem to allow the generic shape class to be used

public:
	GamePiece();
	virtual void Draw(sf::RenderWindow* window);
	virtual void TakeDamage(int damage);
	//void Move(std::stack<GridBox*> path);
	bool MoveToNext(sf::Vector2f desination);
	sf::Vector2i GetPosition();
	void Attack(GamePiece* target);
	std::map<coverDirection, int> coverMap;
	void SetIndex(sf::Vector2f _index);
	bool turnTaken = false;
	bool isDead = false;
};

