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
#include "HeavyPlayerPiece.h"
#include "SniperPlayerPiece.h"
#include "AssaultPlayerPiece.h"
#include "AudioManager.h"
enum gameState {
	playerTurn,
	enemyTurn,
	playerWon,
	enemyWon,
	playerTurnSimulation,
	enemyTurnSimulation
};
class GameManager
{
private:
	sf::RenderWindow* window;
	Grid* grid;
	PlayerPiece* selectedPlayerPiece;
	int selectedPlayerPieceIndex = 0;
	EnemyPiece* selectedEnemyPiece;
	int selectedEnemyPieceIndex = 0;
	std::vector<EnemyPiece*> enemyPieceList;
	std::vector<PlayerPiece*> playerPieceList;
	gameState currentState;
	void PlayerInput();
	//uses helper method to update UI as well as selection itself
	void SelectPlayerPiece(int index);
	InputManager input;
	sf::Clock deltaClock;
	bool endTurn;
	bool CheckEndTurn(bool isPlayer);
	void NextPiece(int index);
	void NextEnemyPiece();
	static GameManager* instance;
	//creates default map
	GameManager(sf::RenderWindow*, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount);
	//creates map from file
	GameManager(sf::RenderWindow*, int screenWidth, int screenHeight, std::string path);
	void LoadMapFromFile(std::string, int, int);
	void SpawnPlayerPiece(int, int);
	void SpawnEnemyPiece(int, int);

public:
	~GameManager();
	void Update();
	void Draw();
	bool gameRunning = true;
	static GameManager* GetInstance();
	static GameManager* CreateInstance(sf::RenderWindow*, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount);
	static GameManager* CreateInstance(sf::RenderWindow*, int screenWidth, int screenHeight, std::string path);
	static void DeleteInstance();
	std::vector<PlayerPiece*> GetPlayerPieces();
	std::vector<EnemyPiece*> GetEnemyPieces();
	Grid* GetGrid();
	sf::Font UIFont;
};

