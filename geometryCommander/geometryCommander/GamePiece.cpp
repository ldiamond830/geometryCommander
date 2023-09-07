#include "GamePiece.h"

void GamePiece::Draw(sf::RenderWindow* window)
{
	//error
}

void GamePiece::TakeDamage(int damage)
{
	health -= damage;

	if (health <= 0) 
	{
		isDead = true;
	}
}
