#include "AssaultPlayerPiece.h"

AssaultPlayerPiece::AssaultPlayerPiece(int xPos, int yPos) : PlayerPiece(xPos, yPos)
{
	attackRange = 4;
	maxDamage = 5;
	minDamage = 2;
	visual->setOutlineThickness(3);
	visual->setOutlineColor(sf::Color(235, 131, 52));
}
