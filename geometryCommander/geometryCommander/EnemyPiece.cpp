#include "EnemyPiece.h"
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
EnemyPiece::EnemyPiece(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	health = 10;
	maxDamage = 3;
	minDamage = 1;
	moveRange = 3;

	visual = new sf::CircleShape(radius);
	visual->setOrigin(radius , radius);
	visual->setPosition(sf::Vector2f(xPos, yPos));
	visual->setFillColor(sf::Color(255, 0, 0));
	//visual->setOutlineThickness(5.0f);
}

EnemyPiece::~EnemyPiece()
{
	delete visual;
}

void EnemyPiece::Draw(sf::RenderWindow* window)
{
	window->draw(*visual);
}
