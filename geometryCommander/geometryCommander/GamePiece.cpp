#include "GamePiece.h"
#include "GridBox.h"
int GamePiece::CalcDamage()
{
	float damage = rand() % maxDamage;

	if (damage < minDamage)
		damage = minDamage;

	return damage;
}

GamePiece::GamePiece()
{
	 xPos = 0;
	 yPos = 0;
	 health = 10;
	 moveRange = 5;
	 maxDamage = 1;
	 minDamage = 0;
}

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

bool GamePiece::MoveToNext(sf::Vector2f desination)
{
	xPos = desination.x;
	yPos = desination.y;
	return true;
}

sf::Vector2i GamePiece::GetPosition()
{
	return sf::Vector2i(xPos, yPos);
}

void GamePiece::Attack(GamePiece* target)
{
	target->TakeDamage(CalcDamage());
}

