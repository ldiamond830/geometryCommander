#include "PlayerPiece.h"

PlayerPiece::PlayerPiece(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	health = 10;
	maxDamage = 3;
	minDamage = 1;
	moveRange = 5;
	attackRange = 7;
	radius = 20;
	visual = new sf::CircleShape(radius);
	visual->setOrigin(radius, radius);
	visual->setPosition(sf::Vector2f(xPos, yPos));
	visual->setFillColor(sf::Color(0, 255, 0));
}

PlayerPiece::~PlayerPiece()
{
	delete visual;
}

void PlayerPiece::Draw(sf::RenderWindow* window)
{
	visual->setPosition(xPos, yPos);
	window->draw(*visual);
}

void PlayerPiece::Select()
{
	visual->setFillColor(sf::Color(0, 0, 255));
}

void PlayerPiece::Deselect()
{
	visual->setFillColor(sf::Color(0, 255, 0));
}
