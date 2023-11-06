#include "HeavyPlayerPiece.h"

HeavyPlayerPiece::HeavyPlayerPiece(int xPos, int yPos) : PlayerPiece(xPos, yPos) {
	health = 15;
	moveRange = 3;
	visual->setOutlineColor(sf::Color::Red);
	visual->setOutlineThickness(3);
	UpdateHealthDisplay();
}
