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
	moveSpeed = 0.01f;
	projectileSpeed = 0.8f;

	bullet = new sf::RectangleShape(sf::Vector2f(20, 20));
	bullet->setFillColor(sf::Color::Magenta);
	bullet->setPosition(sf::Vector2f(xPos, yPos));
	bullet->setOrigin(10, 10);

	grenade = new sf::CircleShape(7.5f);
	grenade->setFillColor(sf::Color::Magenta);
	grenade->setPosition(sf::Vector2f(xPos, yPos));
	grenade->setOrigin(grenade->getRadius()/2.0f, grenade->getRadius() / 2.0f);

	UIText.setOrigin(15, 15);
	UIText.setPosition(sf::Vector2f(xPos, yPos));
	UpdateHealthDisplay();
	UIText.setStyle(sf::Text::Bold);
	UIText.setCharacterSize(25);
	UIText.setFillColor(sf::Color::White);
}

GamePiece::~GamePiece()
{
	delete bullet;
	delete grenade;
}

void GamePiece::Draw(sf::RenderWindow* window)
{
	if (attacking) {
		window->draw(*bullet);
	}
	if (throwingGrenade) {
		window->draw(*grenade);
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
	if (abs(bullet->getPosition().x - end.x) <= 0.1 && abs(bullet->getPosition().y - end.y) <= 0.1) {
		if (!missed) {
			//updates the target's health in the UI when the projectile hits rather than right after the damage caluclation
			target->DisplayDamageTaken();
		}
		
		//reset projectile after hit/miss
		bullet->setPosition(sf::Vector2f(xPos, yPos));
		projectileIterator = 0;
		attacking = false;
		turnFinished = true;
	}
	else {
		bullet->setPosition(sf::Vector2f(std::lerp(start.x, end.x, projectileIterator), std::lerp(start.y, end.y, projectileIterator)));
		projectileIterator += projectileSpeed * dt;
	}
}

void GamePiece::UpdateGrenade(sf::Vector2f start, sf::Vector2f end, float dt)
{
	if (grenadeExploaded) {
		static float timer = 0;
		timer += dt;
		if (timer >= 1) {
			turnFinished = true;
			throwingGrenade = false;
			timer = 0;
		}
	}
	else if (abs(grenade->getPosition().x - end.x) <= 0.1 && abs(grenade->getPosition().y - end.y) <= 0.1) {
		
		grenadeExploaded = true;
		grenade->setScale(sf::Vector2f(7, 7));
		grenade->setOrigin(grenade->getRadius(), grenade->getRadius());
		grenade->setPosition(end);

		auto grid = GameManager::GetInstance()->GetGrid();
		auto box = grid->GetBoxFromPosition((sf::Vector2i)end);

		AudioManager::GetInstance()->PlayGrenadeSound();
		
		for (int x = box->index.x - 1; x <= box->index.x + 1; x++) {
			for (int y = box->index.y - 1; y <= box->index.y + 1; y++) 
			{
				//Don't check out of bounds of the grid.
				if (x < 0 || x > grid->GetWidth() - 1 || y < 0 || y > grid->GetHeight() - 1) {
					continue;
				}

				if (grid->gridBoxes[x][y]->occupyingPiece != nullptr) {
					grid->gridBoxes[x][y]->occupyingPiece->TakeDamage(1);
					grid->gridBoxes[x][y]->occupyingPiece->DisplayDamageTaken();
				}
			}
		}
	}
	else {
		grenade->setPosition(sf::Vector2f(std::lerp(start.x, end.x, projectileIterator), std::lerp(start.y, end.y, projectileIterator)));
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
	else if (throwingGrenade) {
		UpdateGrenade(GetPosition(), targetPosition, dt);
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

void GamePiece::ThrowGrenade(sf::Vector2f pos)
{
	throwingGrenade = true;
	hasGrenade = false;
	targetPosition = pos;
}



