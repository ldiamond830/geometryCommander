#include "PlayerPiece.h"
#include "GameManager.h"

PlayerPiece::PlayerPiece(int xPos, int yPos) : GamePiece(xPos, yPos)
{
	radius = 20;
	visual = new sf::CircleShape(radius);
	visual->setOrigin(radius, radius);
	visual->setPosition(sf::Vector2f(xPos, yPos));
	visual->setFillColor(sf::Color(0, 255, 0));
}

PlayerPiece::~PlayerPiece()
{
	delete visual;
}

void PlayerPiece::Draw(sf::RenderWindow* window)
{
	visual->setPosition(xPos, yPos);
	window->draw(*visual);

	//parent class draws projectiles
	GamePiece::Draw(window);
}

void PlayerPiece::Select()
{
	visual->setFillColor(sf::Color(0, 0, 255));
}

void PlayerPiece::Deselect()
{
	visual->setFillColor(sf::Color(0, 255, 0));
}

bool PlayerPiece::isPositionFlanked(sf::Vector2f pos)
{
	GameManager* gameManagerInstance = GameManager::GetInstance();
	bool flanked = false;
	Grid* grid = gameManagerInstance->GetGrid();
	GridBox* position = grid->gridBoxes[pos.x][pos.y];
	std::map<coverDirection, int> tempMap = grid->GetCoverAtPosition(position);

	if (tempMap.count(coverDirection::UP) != 0) {
		for (EnemyPiece* enemyPiece : gameManagerInstance->GetEnemyPieces())
		{
			//there is a piece behind the position being checked
			if (enemyPiece->GetIndex().y >= position->index.y) {
				flanked = true;
				//checks if the prospective position has cover from multiple angles making the flank not apply
				if (tempMap.count(coverDirection::RIGHT) != 0 && enemyPiece->GetIndex().x > position->index.x) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::LEFT) != 0 && enemyPiece->GetIndex().x < position->index.x) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
	else if (tempMap.count(coverDirection::DOWN) != 0) {
		for (EnemyPiece* enemyPiece : gameManagerInstance->GetEnemyPieces())
		{
			//there is a piece above the position being checked
			if (enemyPiece->GetIndex().y <= position->index.y) {
				flanked = true;
				if (tempMap.count(coverDirection::RIGHT) != 0 && enemyPiece->GetIndex().x > position->index.x) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::LEFT) != 0 && enemyPiece->GetIndex().x < position->index.x) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
	else if (tempMap.count(coverDirection::LEFT) != 0) {
		for (EnemyPiece* enemyPiece : gameManagerInstance->GetEnemyPieces())
		{
			//there is a piece to the right of the position being checked
			if (enemyPiece->GetIndex().x >= position->index.x) {
				flanked = true;
				if (tempMap.count(coverDirection::UP) != 0 && enemyPiece->GetIndex().y > position->index.y) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::DOWN) != 0 && enemyPiece->GetIndex().y < position->index.y) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
	else if (tempMap.count(coverDirection::RIGHT) != 0) {
		for (EnemyPiece* enemyPiece : gameManagerInstance->GetEnemyPieces())
		{
			//there is a piece to the left of the position being checked
			if (enemyPiece->GetIndex().x <= position->index.x) {
				flanked = true;
				if (tempMap.count(coverDirection::UP) != 0 && enemyPiece->GetIndex().y > position->index.y) {
					flanked = false;
				}
				else if (tempMap.count(coverDirection::DOWN) != 0 && enemyPiece->GetIndex().y < position->index.y) {
					flanked = false;
				}
				else {
					//if one piece is flanking ends the loop and moves on without checking any others
					break;
				}
			}
		}
		return flanked;
	}
}