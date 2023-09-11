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

void GamePiece::Move(std::stack<GridBox*> path)
{
	MoveToNext(path.top());
	path.pop();
	if (!path.empty());
	{
		Move(path);
	}

}

bool GamePiece::MoveToNext(GridBox* desination)
{
	xPos = desination->GetCenter().x;
	yPos = desination->GetCenter().y;
}

