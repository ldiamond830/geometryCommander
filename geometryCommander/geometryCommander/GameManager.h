#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "EnemyPiece.h"
#include "GridBox.h"
#include "PlayerPiece.h"
#include <vector>
#include <memory>
#include "InputManager.h"
enum gameState {
	playerTurn,
	enemyTurn
};
class GameManager
{
private:
	sf::RenderWindow* window;
	Grid* grid;
	PlayerPiece* selectedPiece;
	int selectedIndex = 0;
	std::vector<EnemyPiece*> enemyPieceList;
	std::vector<PlayerPiece*> playerPieceList;
	gameState currentState;
	void PlayerInput();
	void SelectPiece(int index);
	InputManager input;

public:
	GameManager(sf::RenderWindow*, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount);
	~GameManager();
	void Update();
	void Draw();
	

};

