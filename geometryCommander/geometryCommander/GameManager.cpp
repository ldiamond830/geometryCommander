#include "GameManager.h"
#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include <string>
GameManager* GameManager::instance = nullptr;
//default map constructor
GameManager::GameManager(sf::RenderWindow* _window, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount)
{
	if (!UIFont.loadFromFile("arial.ttf")) {
		std::cout << "Error loading font";
	}
	
	window = _window;
	grid = new Grid();

	for (unsigned int i = 0; i < playerPieceCount; i++) {
		SpawnPlayerPiece(i, 0);
	}

	for (unsigned int i = 0; i < enemyPieceCount; i++) {
		SpawnEnemyPiece(i, columnSize - 1);
	}

	SelectPlayerPiece(0);
	
	selectedEnemyPiece = enemyPieceList[selectedEnemyPieceIndex];
	currentState = gameState::playerTurn;
	input = InputManager();
}

//map from file constructor
GameManager::GameManager(sf::RenderWindow* _window, int screenWidth, int screenHeight, std::string path) {
	if (!UIFont.loadFromFile("arial.ttf")) {
		std::cout << "Error loading font";
	}
	LoadMapFromFile(path, screenWidth, screenHeight);
	window = _window;
	SelectPlayerPiece(0);
	selectedEnemyPiece = enemyPieceList[selectedEnemyPieceIndex];
	currentState = gameState::playerTurn;
	input = InputManager();
}

void GameManager::LoadMapFromFile(std::string path, int screenWidth, int screenHeight)
{
	std::ifstream fileReader(path);
	if (fileReader.good()) {
		std::string line;
		//first two lines of the map file are size values
		std::getline(fileReader, line);
		int rowSize = std::stoi(line);
		std::getline(fileReader, line);
		int columnSize = std::stoi(line);
		grid = new Grid(screenWidth, screenHeight, rowSize, columnSize);

		//loop through the remaining lines of the file assigning the values of each box based on the character in the line
		for (unsigned int y = 0; y < columnSize; y++) {
			std::getline(fileReader, line);
			for (unsigned int x = 0; x < rowSize; x++) {
				switch (line[x])
				{
				case 'h':
					grid->gridBoxes[x][y]->SetType(halfCover);
						break;
				case 'f':
					grid->gridBoxes[x][y]->SetType(fullCover);
					break;
				case 'p':
					SpawnPlayerPiece(x, y);
					break;
				case 'e':
					SpawnEnemyPiece(x, y);
					break;
				}
			}
		}

	}
	//file inputed empty
	else {
		std::cout << "error";
	}
	
}

void GameManager::SpawnPlayerPiece(int x, int y)
{
	PlayerPiece* playerPiece;
	float rng = rand();
	if (rng < 0.35) {
		playerPiece = new AssaultPlayerPiece(grid->gridBoxes[x][y]->GetCenter().x, grid->gridBoxes[x][y]->GetCenter().y);
	}
	else if(rng < 0.67) {
		playerPiece = new SniperPlayerPiece(grid->gridBoxes[x][y]->GetCenter().x, grid->gridBoxes[x][y]->GetCenter().y);
	}
	else {
		playerPiece = new HeavyPlayerPiece(grid->gridBoxes[x][y]->GetCenter().x, grid->gridBoxes[x][y]->GetCenter().y);
	}

	grid->UpdateOccupyingPiece(grid->gridBoxes[x][y], playerPiece);
	playerPiece->SetFont(&UIFont);
	playerPieceList.push_back(playerPiece);
}

void GameManager::SpawnEnemyPiece(int x, int y)
{
	EnemyPiece* enemyPiece = new EnemyPiece(grid->gridBoxes[x][y]->GetCenter().x, grid->gridBoxes[x][y]->GetCenter().y);
	grid->UpdateOccupyingPiece(grid->gridBoxes[x][y], enemyPiece);
	enemyPiece->SetFont(&UIFont);
	enemyPieceList.push_back(enemyPiece);
}

Grid* GameManager::GetGrid()
{
	return grid;
}

GameManager::~GameManager()
{
	delete grid;
	for (PlayerPiece* p : playerPieceList) {
		delete p;
	}
	for (EnemyPiece* e : enemyPieceList) {
		delete e;
	}
}

void GameManager::Update()
{
	switch (currentState) {
	
		//wait for player to input move
		case gameState::playerTurn:
		if (CheckEndTurn(true)) {
			currentState = gameState::enemyTurn;
			//hides the blue highlight color for the duration of the enemy turn 
			grid->ClearBoxesInRange();
			selectedPlayerPiece->Deselect();
			for (PlayerPiece* piece : playerPieceList) {
				piece->turnTaken = false;
			}
		}

		//read player input
		PlayerInput();
		break;

		//visualize player move
		case gameState::playerTurnSimulation: {
			selectedPlayerPiece->SimulateAction();
			//done simulating
			if (selectedPlayerPiece->turnFinished) {
				//wait for player input for the next player piece
				NextPiece(1);
				currentState = gameState::playerTurn;
			}
		}
		break;

		//run AI behavior tree
		case gameState::enemyTurn:
		//calculate paths to each box for the current piece
		grid->ClearBoxesInRange();
		grid->CalculatePaths(selectedEnemyPiece, selectedEnemyPiece->GetMovementRange());

		selectedEnemyPiece->TakeTurn();
		currentState = enemyTurnSimulation;
		break;

		//visualize enemy turn to player
		case gameState::enemyTurnSimulation:
			selectedEnemyPiece->SimulateAction();

			if (selectedEnemyPiece->turnFinished) {
				//start turn for next enemy piece
				NextEnemyPiece();
				//if the last enemy turn has finished simulating
				if (CheckEndTurn(false)) {
					currentState = gameState::playerTurn;
					//reveal the blue color on the selected piece
					selectedPlayerPiece->Select();
					//clear boxes in range of the most recent enemy piece
					grid->ClearBoxesInRange();

					grid->ShowBoxesInRange(selectedPlayerPiece, selectedPlayerPiece->GetMovementRange());
					for (EnemyPiece* piece : enemyPieceList) {
						piece->turnTaken = false;
					}
				}
				//otherwise run the next enemy's behavior tree
				else {
					currentState = gameState::enemyTurn;
				}
			}
			break;
	}

	//check for dead pieces at the end of each turn
	for (unsigned int i = 0; i < playerPieceList.size(); i++)
	{
		if (playerPieceList[i]->isDead) {
			grid->GetBoxFromOccupyingPiece(playerPieceList[i])->occupyingPiece = nullptr;
			delete playerPieceList[i];
			playerPieceList.erase(playerPieceList.begin() + i);

			if (playerPieceList.size() == 0) {
				currentState = enemyWon;
			}
			//if there are remaining player pieces and the selected piece has just died select the next one
			else if (selectedPlayerPiece == playerPieceList[i]) {
				NextPiece(1);
			}
		}
	}

	for (unsigned int i = 0; i < enemyPieceList.size(); i++)
	{
		if (enemyPieceList[i]->isDead) {
			grid->GetBoxFromOccupyingPiece(enemyPieceList[i])->occupyingPiece = nullptr;
			delete enemyPieceList[i];
			enemyPieceList.erase(enemyPieceList.begin() + i);
			if (enemyPieceList.size() == 0) {
				currentState = playerWon;
			}
			//if there are remaining enemy pieces and the selected piece has just died select the next one
			else if (selectedEnemyPiece == enemyPieceList[i]) {
				NextEnemyPiece();
			}
		}
	}
}

void GameManager::Draw()
{
	grid->Draw(window);

	for (EnemyPiece* enemy : enemyPieceList) {
		enemy->Draw(window);
	}

	for (PlayerPiece* playerPiece : playerPieceList) {
		playerPiece->Draw(window);
	}
	
}

//return singleton instance
GameManager* GameManager::GetInstance()
{
	if (instance == nullptr) {
		throw std::invalid_argument("Instance not initialized");
	}

	return instance;
}

//creates default map
GameManager* GameManager::CreateInstance(sf::RenderWindow* window, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount)
{
	if (instance == nullptr) {
		instance = new GameManager(window, screenWidth, screenHeight, rowSize, columnSize, playerPieceCount, enemyPieceCount);
	}
	return instance;
}

//creates custom map from file
GameManager* GameManager::CreateInstance(sf::RenderWindow* window, int screenWidth, int screenHeight, std::string path)
{
	if (instance == nullptr) {
		instance = new GameManager(window, screenWidth, screenHeight, path);
	}
	return instance;
}

void GameManager::DeleteInstance()
{
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

std::vector<PlayerPiece*> GameManager::GetPlayerPieces()
{
	return playerPieceList;
}

std::vector<EnemyPiece*> GameManager::GetEnemyPieces()
{
	return enemyPieceList;
}

void GameManager::PlayerInput()
{
	input.PlayerInput();

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		auto clickedBox = grid->GetBoxFromPosition(sf::Mouse::getPosition(*window));
		//left clicking on a box in move range will move the current piece to that box
		if (clickedBox != nullptr) {
			if (clickedBox->inPlayerMoveRange) {
				//std::cout << "Start: (X=" << selectedPlayerPiece->GetIndex().x << ", Y=" << selectedPlayerPiece->GetIndex().y << ") End: (X=" << clickedBox->index.x << ", Y=" << clickedBox->index.y << ")" << std::endl;
				grid->MovePiece(grid->gridBoxes[selectedPlayerPiece->GetIndex().x][selectedPlayerPiece->GetIndex().y], clickedBox);
				selectedPlayerPiece->turnTaken = true;
				currentState = gameState::playerTurnSimulation;
				AudioManager::GetInstance()->PlayMoveSound();
			}
			//if the clicked box has a enemy piece, calls an attack
			else if (clickedBox->GetType() == gridBoxType::occupied && dynamic_cast<EnemyPiece*>(clickedBox->occupyingPiece) != nullptr)
			{
				selectedPlayerPiece->Attack(clickedBox->occupyingPiece);
				selectedPlayerPiece->turnTaken = true;
				currentState = gameState::playerTurnSimulation;
			}
		}
	}
	//mouse click takes priority over switching piece
	//Q and E cycle the currently selected piece
	else if (input.isKeyReleased(sf::Keyboard::Q)) {
		NextPiece(-1);
	}
	else if (input.isKeyReleased(sf::Keyboard::E)) {
		NextPiece(1);
	}
}

void GameManager::SelectPlayerPiece(int index)
{
	if (index < 0) {
		index = 0;
	}

	selectedPlayerPieceIndex = index;

	//stops previosu selected piece from being highlighted
	if (selectedPlayerPiece != nullptr) {
		selectedPlayerPiece->Deselect();
	}
	
	selectedPlayerPiece = playerPieceList[selectedPlayerPieceIndex];
	selectedPlayerPiece->Select();

	//highlights gridboxes in range of the selected piece's index
	grid->ClearBoxesInRange();
	grid->ShowBoxesInRange(selectedPlayerPiece, selectedPlayerPiece->GetMovementRange());
}


bool GameManager::CheckEndTurn(bool isPlayer)
{
	endTurn = true;

	if (isPlayer) {
		for (unsigned int i = 0; i < playerPieceList.size(); i++) {
			if (!playerPieceList[i]->turnTaken) {
				endTurn = false;
			}
		}
	}

	else {
		for (unsigned int i = 0; i < enemyPieceList.size(); i++) {
			if (!enemyPieceList[i]->turnTaken) {
				endTurn = false;
			}
		}
	}
	return endTurn;
}

void GameManager::NextPiece(int iterator)
{
	do {
		SelectPlayerPiece((selectedPlayerPieceIndex + iterator) % playerPieceList.size());
	} while (selectedPlayerPiece->turnTaken && !CheckEndTurn(true));
}

void GameManager::NextEnemyPiece()
{
	selectedEnemyPieceIndex = (selectedEnemyPieceIndex + 1) % (enemyPieceList.size());
	selectedEnemyPiece = enemyPieceList[selectedEnemyPieceIndex];
}
