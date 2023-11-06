#include "SniperPlayerPiece.h"

SniperPlayerPiece::SniperPlayerPiece(int xPos, int yPos) : PlayerPiece(xPos, yPos)
{
	health = 7;
	attackRange = 10;
	visual->setOutlineThickness(3);
	visual->setOutlineColor(sf::Color::Magenta);
	UpdateHealthDisplay();
}
