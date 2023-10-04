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
	bool endTurn;
	bool CheckEndTurn(bool isPlayer);
	void NextPiece(int index);
	static GameManager* instance;
	GameManager(sf::RenderWindow*, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount);

public:
	~GameManager();
	void Update();
	void Draw();
	bool gameRunning = true;
	static GameManager* GetInstance();
	static GameManager* CreateInstance(sf::RenderWindow*, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount);
	static void DeleteInstance();
	std::vector<PlayerPiece*> GetPlayerPieces();
};

