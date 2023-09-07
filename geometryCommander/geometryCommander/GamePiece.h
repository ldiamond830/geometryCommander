#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class GamePiece
{
protected:
	float xPos;
	float yPos;
	int health;
	int moveRange;
	int maxDamage;
	int minDamage;
	bool isDead = false;
	//each child class will need to define visual since SMFL doesn't seem to allow the generic shape class to be used

public:
	virtual void Draw(sf::RenderWindow* window);
	virtual void TakeDamage(int damage);
	void Move(sf::Vector2f);

};

