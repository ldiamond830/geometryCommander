#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <string>
GameManager* GameManager::instance = nullptr;
GameManager::GameManager(sf::RenderWindow* _window, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount)
{
	if (!UIFont.loadFromFile("arial.ttf")) {
		std::cout << "Error loading font";
	}
	clock.restart();
	timestep = sf::milliseconds(50);

	window = _window;
	grid = new Grid();
	for (unsigned int i = 0; i < playerPieceCount; i++) {
		PlayerPiece* playerPiece = new PlayerPiece(grid->gridBoxes[i][0]->GetCenter().x, grid->gridBoxes[i][0]->GetCenter().y);
		grid->UpdateOccupyingPiece(grid->gridBoxes[i][0], playerPiece);
		playerPiece->SetFont(&UIFont);
		playerPieceList.push_back(playerPiece);
	}
	for (unsigned int i = 0; i < enemyPieceCount; i++) {
		EnemyPiece* enemyPiece = new EnemyPiece(grid->gridBoxes[i][columnSize - 1]->GetCenter().x, grid->gridBoxes[i][columnSize - 1]->GetCenter().y);
		grid->UpdateOccupyingPiece(grid->gridBoxes[i][columnSize - 1], enemyPiece);
		enemyPiece->SetFont(&UIFont);
		enemyPieceList.push_back(enemyPiece);
	}
	SelectPlayerPiece(0);
	selectedEnemyPiece = enemyPieceList[selectedEnemyPieceIndex];
	currentState = gameState::playerTurn;
	input = InputManager();
}

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
					{
						PlayerPiece* playerPiece = new PlayerPiece(grid->gridBoxes[x][y]->GetCenter().x, grid->gridBoxes[x][y]->GetCenter().y);
						grid->UpdateOccupyingPiece(grid->gridBoxes[x][y], playerPiece);
						playerPiece->SetFont(&UIFont);
						playerPieceList.push_back(playerPiece);
					}
					break;
				case 'e':
					{
						EnemyPiece* enemyPiece = new EnemyPiece(grid->gridBoxes[x][y]->GetCenter().x, grid->gridBoxes[x][y]->GetCenter().y);
						grid->UpdateOccupyingPiece(grid->gridBoxes[x][y], enemyPiece);
						enemyPiece->SetFont(&UIFont);
						enemyPieceList.push_back(enemyPiece);
					}
					break;
				}
			}
		}

	}
	else {
		std::cout << "error";
	}
	
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
	sf::Time dt = clock.restart();
	totalTime += dt;
	float floatDt = dt.asSeconds();

	switch (currentState) {
	
		//wait for player to input move
		case gameState::playerTurn:
		if (!CheckEndTurn(true)) {
			currentState = gameState::enemyTurn;
			//hides the blue color for the duration of the enemy turn 
			selectedPlayerPiece->Deselect();
			for (PlayerPiece* piece : playerPieceList) {
				piece->turnTaken = false;
			}
		}

		PlayerInput();
		break;

		//visualize player move
		case gameState::playerTurnSimulation: {
			selectedPlayerPiece->SimulateAction();
			//done simulating
			if (selectedPlayerPiece->turnFinished) {
				NextPiece(1);
				currentState = gameState::playerTurn;
			}
		}
		break;

		//run AI behavior tree
		case gameState::enemyTurn:
		selectedEnemyPiece->TakeTurn();
		currentState = enemyTurnSimulation;
		break;

		//visualize enemy turn to player
		case gameState::enemyTurnSimulation:
			selectedEnemyPiece->SimulateAction();

			if (selectedEnemyPiece->turnFinished) {
				selectedEnemyPieceIndex = (selectedEnemyPieceIndex + 1) % (enemyPieceList.size());
				selectedEnemyPiece = enemyPieceList[selectedEnemyPieceIndex];
				//if the last enemy turn has finished simulating
				if (!CheckEndTurn(false)) {
					currentState = gameState::playerTurn;
					//reveal the blue color on the selected piece
					selectedPlayerPiece->Select();
					for (EnemyPiece* piece : enemyPieceList) {
						piece->turnTaken = false;
					}
				}
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

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr) {
		throw std::invalid_argument("Instance not initialized");
	}

	return instance;
}

GameManager* GameManager::CreateInstance(sf::RenderWindow* window, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount)
{
	if (instance == nullptr) {
		instance = new GameManager(window, screenWidth, screenHeight, rowSize, columnSize, playerPieceCount, enemyPieceCount);
	}
	return instance;
}

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
		if (clickedBox != nullptr) {
			if (clickedBox->GetType() == gridBoxType::empty) {
				grid->MovePiece(grid->GetBoxFromOccupyingPiece(selectedPlayerPiece), clickedBox);
				selectedPlayerPiece->turnTaken = true;
				//NextPiece(1);
				currentState = gameState::playerTurnSimulation;
			}
			else if (clickedBox->GetType() == gridBoxType::occupied && dynamic_cast<EnemyPiece*>(clickedBox->occupyingPiece) != nullptr)
			{
				selectedPlayerPiece->Attack(clickedBox->occupyingPiece);
				selectedPlayerPiece->turnTaken = true;
				currentState = gameState::playerTurnSimulation;
			}
		}
	}

	if (input.isKeyReleased(sf::Keyboard::Q)) {
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
	if (selectedPlayerPiece != nullptr) {
		selectedPlayerPiece->Deselect();
	}
	
	selectedPlayerPiece = playerPieceList[selectedPlayerPieceIndex];
	selectedPlayerPiece->Select();
}

bool GameManager::CheckEndTurn(bool isPlayer)
{
	endTurn = false;
	if (isPlayer) {
		for (unsigned int i = 0; i < playerPieceList.size(); i++) {
			if (!playerPieceList[i]->turnTaken) {
				endTurn = true;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < enemyPieceList.size(); i++) {
			if (!enemyPieceList[i]->turnTaken) {
				endTurn = true;
			}
		}
	}
	return endTurn;
}

void GameManager::NextPiece(int iterator)
{
	do {
		SelectPlayerPiece((selectedPlayerPieceIndex + iterator) % playerPieceList.size());
	} while (selectedPlayerPiece->turnTaken && CheckEndTurn(true));
}
