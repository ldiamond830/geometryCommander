#include "GameManager.h"
#include <iostream>
GameManager* GameManager::instance = nullptr;
GameManager::GameManager(sf::RenderWindow* _window, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount)
{
	/*
	if (!UIFont.loadFromFile("vgafix.ttf")) {
		std::cout << "Error loading font";
	}
	*/
	window = _window;
	grid = new Grid(screenWidth, screenHeight, rowSize, columnSize);
	for (unsigned int i = 0; i < playerPieceCount; i++) {
		PlayerPiece* playerPiece = new PlayerPiece(grid->gridBoxes[i][0]->GetCenter().x, grid->gridBoxes[i][0]->GetCenter().y);
		grid->UpdateOccupyingPiece(grid->gridBoxes[i][0], playerPiece);
		playerPieceList.push_back(playerPiece);
	}
	for (unsigned int i = 0; i < enemyPieceCount; i++) {
		EnemyPiece* enemyPiece = new EnemyPiece(grid->gridBoxes[i][columnSize - 1]->GetCenter().x, grid->gridBoxes[i][columnSize - 1]->GetCenter().y);
		grid->UpdateOccupyingPiece(grid->gridBoxes[i][columnSize - 1], enemyPiece);
		enemyPieceList.push_back(enemyPiece);
	}
	SelectPlayerPiece(0);
	selectedEnemyPiece = enemyPieceList[selectedEnemyPieceIndex];
	currentState = gameState::playerTurn;
	input = InputManager();
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
