#pragma once
#include "GamePiece.h"
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class EnemyPiece : public GamePiece
{
public:
	EnemyPiece(int xPos, int yPos);
	~EnemyPiece();
	void Draw(sf::RenderWindow* window) override;
private:
	sf::CircleShape* visual;
	float radius = 10;
};

