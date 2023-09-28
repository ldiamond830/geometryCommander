#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow* _window, int screenWidth, int screenHeight, int rowSize, int columnSize, int playerPieceCount, int enemyPieceCount)
{
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
	EnemyPiece* enemyPiece = new EnemyPiece(grid->gridBoxes[6][1]->GetCenter().x, grid->gridBoxes[6][1]->GetCenter().y);
	grid->UpdateOccupyingPiece(grid->gridBoxes[6][1], enemyPiece);
	enemyPieceList.push_back(enemyPiece);
	SelectPiece(0);
	//player always goes first, at least for now
	currentState = gameState::playerTurn;
	input = InputManager();
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
	
		case gameState::playerTurn:
		PlayerInput();
		if (!CheckEndTurn(true)) {
			currentState = gameState::enemyTurn;
			//removes the blue color for the duration of the enemy turn 
			selectedPiece->Deselect();
			for (PlayerPiece* piece : playerPieceList) {
				piece->turnTaken = false;
			}
		}
		break;

		case gameState::enemyTurn:
		if (!CheckEndTurn(false)) {
			currentState = gameState::playerTurn;
			selectedPiece->Select();
			for (EnemyPiece* piece : enemyPieceList) {
				piece->turnTaken = false;
			}
		}
		break;
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

void GameManager::PlayerInput()
{
	input.PlayerInput();
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		auto clickedBox = grid->GetBoxFromPosition(sf::Mouse::getPosition(*window));
		if (clickedBox != nullptr) {
			if (clickedBox->GetType() == gridBoxType::empty) {
				grid->MovePiece(grid->GetBoxFromPosition(selectedPiece->GetPosition()), clickedBox);
				selectedPiece->turnTaken = true;
				NextPiece(1);
			}
			else if (clickedBox->GetType() == gridBoxType::occupied && dynamic_cast<EnemyPiece*>(clickedBox->occupyingPiece) != nullptr)
			{
				selectedPiece->Attack(clickedBox->occupyingPiece);
				selectedPiece->turnTaken = true;
				NextPiece(1);
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

void GameManager::SelectPiece(int index)
{
	if (index < 0) {
		index = 0;
	}
	selectedIndex = index;
	if (selectedPiece != nullptr) {
		selectedPiece->Deselect();
	}
	
	selectedPiece = playerPieceList[selectedIndex];
	selectedPiece->Select();
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

bool GameManager::CheckEnemyEndTurn(std::vector<EnemyPiece*> list)
{
	endTurn = true;
	for (unsigned int i = 0; i < list.size(); i++) {
		if (!list[i]->turnTaken) {
			endTurn = false;
		}
	}
	return endTurn;
}

void GameManager::NextPiece(int index)
{
	do {
		SelectPiece((selectedIndex + index) % playerPieceList.size());
	} while (selectedPiece->turnTaken && CheckEndTurn(true));
}
