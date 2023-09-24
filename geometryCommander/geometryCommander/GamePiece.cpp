#include "GamePiece.h"
#include "GridBox.h"
//#include "Utils.h"
#include <iostream>
int GamePiece::CalcDamage()
{
	float damage = rand() % maxDamage;

	if (damage < minDamage)
		damage = minDamage;

	return damage;
}

bool GamePiece::ChanceToHit(GamePiece* target)
{
	float chanceToHit = 75;
	//move both manhatann distance calculations to a utils class
	float dist = abs(index.x - target->index.x) + abs(index.y - target->index.y);
	bool flank = false;
	//point blank
	if (dist < 2) {
		chanceToHit += 20;
	}
	else {
		//target is in cover
		if (!target->coverMap.empty()) {
			//attacker is to the left of the target
			if (index.x < target->index.x) {

				if (target->coverMap.count(coverDirection::LEFT) != 0) {
					chanceToHit -= coverMap[coverDirection::LEFT];
				}
				else
				{
					//attacker is above target
					if (index.y < target->index.y) {
						if (target->coverMap.count(coverDirection::UP) != 0) {
							chanceToHit -= coverMap[coverDirection::UP];
						}
						else {
							flank = true;
						}
					}
					//attack is below target
					else if (index.y > target->index.y) {
						if (target->coverMap.count(coverDirection::DOWN) != 0) {
							chanceToHit -= coverMap[coverDirection::DOWN];
						}
						else {
							flank = true;
						}
					}
					//attack and target are in the same row
					else {
						flank = true;
					}
				}
			}
			//attacker is to the right of the target
			else if (index.x > target->index.x) {
				if (target->coverMap.count(coverDirection::RIGHT) != 0) {
					chanceToHit -= coverMap[coverDirection::RIGHT];
				}
				else
				{
					if (index.y < target->index.y) {
						if (target->coverMap.count(coverDirection::UP) != 0) {
							chanceToHit -= coverMap[coverDirection::UP];
						}
						else {
							flank = true;
						}
					}
					//attack is below target
					else if (index.y > target->index.y) {
						if (target->coverMap.count(coverDirection::DOWN) != 0) {
							chanceToHit -= coverMap[coverDirection::DOWN];
						}
						else {
							flank = true;
						}
					}
					//attack and target are in the same row
					else {
						flank = true;
					}
				}
			}
			//attacker and target are in the same column
			else {
				if (index.y < target->index.y) {
					if (target->coverMap.count(coverDirection::UP) != 0) {
						chanceToHit -= coverMap[coverDirection::UP];
					}
					else {
						flank = true;
					}
				}
				//attack is below target
				else if (index.y > target->index.y) {
					if (target->coverMap.count(coverDirection::DOWN) != 0) {
						chanceToHit -= coverMap[coverDirection::DOWN];
					}
					else {
						flank = true;
					}
				}
				//no need to check if attacker and target are in the same row since that would require then to occupy the same gridbox which isn't allowed
			}
		}
		//target is not in cover
		else {
			chanceToHit += 15;
		}
	}
	
	if (flank) {
		chanceToHit += 15;
	}
	float rng = rand() % 100;

	return chanceToHit > rng;
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
	if (ChanceToHit(target)) {
		target->TakeDamage(CalcDamage());
	}
	else {
		//miss
	}

}

void GamePiece::SetIndex(sf::Vector2f _index)
{
	index = _index;
}

