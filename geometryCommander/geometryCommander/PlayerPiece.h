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


private:
	sf::CircleShape* visual;
	float radius = 10;
};

