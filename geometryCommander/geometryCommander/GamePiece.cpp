#include "GamePiece.h"
#include "GridBox.h"
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

void GamePiece::Move(std::stack<GridBox*> path)
{
	MoveToNext(path.top());
	path.pop();
	if (!path.empty());
	{
		//Move(path);
	}

}

bool GamePiece::MoveToNext(GridBox* desination)
{
	xPos = desination->GetCenter().x;
	yPos = desination->GetCenter().y;
	return true;
}

