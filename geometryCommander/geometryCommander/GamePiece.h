#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stack>
#include <queue>
#include <map>
enum coverDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class GamePiece
{
protected:
	float xPos;
	float yPos;
	int health;
	int moveRange;
	int attackRange;
	int maxDamage;
	int minDamage;
	float radius;
	bool atDestination = false;
	int CalcDamage();
	bool ChanceToHit(GamePiece*);
	sf::Vector2f targetPosition;
	GamePiece* target;
	int accuracy;
	bool missed;

	sf::Vector2f index;
	
	std::stack<sf::Vector2f>* path;
	bool moving = false;
	bool attacking = false;
	sf::RectangleShape* projectile;
	bool MoveToNext(sf::Vector2f);
	void UpdateProjectile(sf::Vector2f, sf::Vector2f);
	float projectileIterator;
	float movementIterator;
	sf::Text UIText;
	void UpdateHealthDisplay();
	//sfml doesn't allow creation of variables using the abstract classes Drawable or Shape so each child needs to define it's own visual in order to allow for different shapes being used

public:
	GamePiece();
	GamePiece(int, int);
	~GamePiece();
	virtual void Draw(sf::RenderWindow* window);
	virtual void TakeDamage(int damage);
	//void Move(std::stack<GridBox*> path);
	void StartMove(std::stack<sf::Vector2f>* path);
	sf::Vector2f GetPosition();
	void Attack(GamePiece* target);
	std::map<coverDirection, int> coverMap;
	void SetIndex(sf::Vector2f _index);
	bool turnTaken = false;
	bool isDead = false;
	sf::Vector2f GetIndex();
	int GetHealth();
	bool turnFinished;
	virtual void SimulateAction();
	void SetFont(sf::Font* _UIFont);
	int GetMovementRange();
	void SetHealth(int);
};

