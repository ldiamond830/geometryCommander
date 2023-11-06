#pragma once
#include "GamePiece.h"
class PlayerPiece : public GamePiece
{
public:
	PlayerPiece(int xPos, int yPos);
	~PlayerPiece();
	void Draw(sf::RenderWindow* window) override;
	void Select();
	void Deselect();
	bool isPositionFlanked(sf::Vector2f);

protected:
	sf::CircleShape* visual;
};

