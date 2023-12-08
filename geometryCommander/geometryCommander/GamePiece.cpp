#include "GamePiece.h"
//#include "GridBox.h"
//#include "Utils.h"
#include <iostream>
#include "GameManager.h"
#include <cmath>
int GamePiece::CalcDamage()
{
	float damage = rand() % maxDamage;

	if (damage < minDamage)
		damage = minDamage;

	return damage;
}

bool GamePiece::ChanceToHit(GamePiece* target)
{
	float chanceToHit = accuracy;
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
					chanceToHit -= target->coverMap[coverDirection::LEFT];
				}
				else
				{
					//attacker is above target
					if (index.y < target->index.y) {
						if (target->coverMap.count(coverDirection::UP) != 0) {
							chanceToHit -= target->coverMap[coverDirection::UP];
						}
						else {
							flank = true;
						}
					}
					//attack is below target
					else if (index.y > target->index.y) {
						if (target->coverMap.count(coverDirection::DOWN) != 0) {
							chanceToHit -= target->coverMap[coverDirection::DOWN];
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
				if (target->coverMap.contains(coverDirection::RIGHT)) {
					chanceToHit -= target->coverMap[coverDirection::RIGHT];
				}
				else
				{
					if (index.y < target->index.y) {
						if (target->coverMap.contains(coverDirection::UP)) {
							chanceToHit -= target->coverMap[coverDirection::UP];
						}
						else {
							flank = true;
						}
					}
					//attack is below target
					else if (index.y > target->index.y) {
						if (target->coverMap.contains(coverDirection::DOWN)) {
							chanceToHit -= target->coverMap[coverDirection::DOWN];
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
					if (target->coverMap.contains(coverDirection::UP)) {
						chanceToHit -= target->coverMap[coverDirection::UP];
					}
					else {
						flank = true;
					}
				}
				//attack is below target
				else if (index.y > target->index.y) {
					if (target->coverMap.contains(coverDirection::DOWN)) {
						chanceToHit -= target->coverMap[coverDirection::DOWN];
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
	std::cout << "Chance to hit: " << chanceToHit << std::endl << "rng: " << rng << std::endl;
	return chanceToHit > rng;
}


GamePiece::GamePiece(int _xPos, int _yPos)
{
	xPos = _xPos;
	yPos = _yPos;
	isDead = false;
	health = 10;
	moveRange = 5;
	attackRange = 9;
	maxDamage = 5;
	minDamage = 3;
	accuracy = 75;
	moveSpeed = 0.005f;
	projectileSpeed = 0.8f;

	projectile = new sf::RectangleShape(sf::Vector2f(20, 20));
	projectile->setFillColor(sf::Color::Magenta);
	projectile->setPosition(sf::Vector2f(xPos, yPos));
	projectile->setOrigin(10, 10);

	UIText.setOrigin(15, 15);
	UIText.setPosition(sf::Vector2f(xPos, yPos));
	UpdateHealthDisplay();
	UIText.setStyle(sf::Text::Bold);
	UIText.setCharacterSize(25);
	UIText.setFillColor(sf::Color::White);
}

GamePiece::~GamePiece()
{
	delete projectile;
}

void GamePiece::Draw(sf::RenderWindow* window)
{
	if (attacking) {
		window->draw(*projectile);
	}
	window->draw(UIText);
}

void GamePiece::TakeDamage(int damage)
{
	health -= damage;
}

void GamePiece::StartMove(std::stack<sf::Vector2f>* _path)
{
	path = _path;
	moving = true;
}

bool GamePiece::MoveToNext(sf::Vector2f desination, float dt)
{
	
	if (xPos != desination.x || yPos != desination.y) {
		if (abs(xPos - desination.x) <= 0.1) {
			xPos = desination.x;
		}
		else {
			xPos = std::lerp(xPos, desination.x, movementIterator);
		}

		if (abs(yPos - desination.y) <= 0.1) {
			yPos = desination.y;
		}
		else {
			yPos = std::lerp(yPos, desination.y, movementIterator);
		}

		movementIterator += moveSpeed * dt;
	}
	else {
		movementIterator = 0;
		return true;
	}
	return false;
}

void GamePiece::UpdateProjectile(sf::Vector2f start, sf::Vector2f end, float dt)
{
	if (abs(projectile->getPosition().x - end.x) <= 0.1 && abs(projectile->getPosition().y - end.y) <= 0.1) {
		if (!missed) {
			//updates the target's health in the UI when the projectile hits rather than right after the damage caluclation
			target->DisplayDamageTaken();
		}
		
		//reset projectile after hit/miss
		projectile->setPosition(sf::Vector2f(xPos, yPos));
		projectileIterator = 0;
		attacking = false;
		turnFinished = true;
	}
	else {
		projectile->setPosition(sf::Vector2f(std::lerp(start.x, end.x, projectileIterator), std::lerp(start.y, end.y, projectileIterator)));
		projectileIterator += projectileSpeed * dt;
	}
}

void GamePiece::UpdateHealthDisplay()
{
	UIText.setString(std::to_string(health));
	if (health < 10) {
		UIText.setOrigin(9, 15);
	}
}

void GamePiece::DisplayDamageTaken()
{
	
	UpdateHealthDisplay();

	if (health <= 0)
	{
		isDead = true;
		AudioManager::GetInstance()->PlayDieSound();
	}
	//non lethal damage
	else {
		AudioManager::GetInstance()->PlayHitSound();
	}
}

sf::Vector2f GamePiece::GetPosition()
{
	return sf::Vector2f((float)xPos, (float)yPos);
}

void GamePiece::Attack(GamePiece* _target)
{
	missed = false;
	targetPosition = _target->GetPosition();
	target = _target;
	if (ChanceToHit(_target)) {
		_target->TakeDamage(CalcDamage());
		attacking = true;
	}
	else {
		//miss shows the attack going wide
		targetPosition.x += rand() % 50 + 20;
		attacking = true;
		missed = true;
	}
	AudioManager::GetInstance()->PlayAttackSound();
}

void GamePiece::SetIndex(sf::Vector2f _index)
{
	index = _index;
}

sf::Vector2f GamePiece::GetIndex()
{
	return index;
}

int GamePiece::GetHealth()
{
	return health;
}

void GamePiece::SimulateAction(float dt)
{
	turnFinished = false;
	if (moving){
		if (MoveToNext(path->top(), dt)) {
			path->pop();
			AudioManager::GetInstance()->PlayMoveSound();
		}

		if (path->empty()) {
			moving = false;
			turnFinished = true;
		}
		UIText.setPosition(sf::Vector2f(xPos, yPos));
	}
	else if (attacking) {
		UpdateProjectile(GetPosition(), targetPosition, dt);
	}
}

void GamePiece::SetFont(sf::Font* _UIFont)
{
	UIText.setFont(*_UIFont);
}

int GamePiece::GetMovementRange()
{
	return moveRange;
}

void GamePiece::SetHealth(int newHealth)
{
	health = newHealth;
}



