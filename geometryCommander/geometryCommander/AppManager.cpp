#include "AppManager.h"
#include <iostream>

AppManager* AppManager::instance = nullptr;

AppManager::AppManager(sf::RenderWindow* _window, int _screenWidth, int _screenHeight)
{
	window = _window;
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	currentState = title;
	input = InputManager();
	if (!UIFont.loadFromFile("arial.ttf")) {
		std::cout << "Error loading font";
	}

	InitTitle();
}


void AppManager::StartGame()
{
	gameManager = GameManager::CreateInstance(window, screenHeight, screenHeight, 10, 10, 4, 4);
	currentState = game;
}

void AppManager::InitTitle()
{
	sf::Text titleText;
	titleText.setFont(UIFont);
	titleText.setString("GEOMETRY COMMANDER");
	titleText.setCharacterSize(50);
	titleText.setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(&titleText);
	titleText.setPosition(screenWidth / 2.0f, 50.0f);

	sf::Text enterText;
	enterText.setFont(UIFont);
	enterText.setString("PRESS ENTER");
	enterText.setCharacterSize(30);
	enterText.setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(&enterText);
	enterText.setPosition(screenWidth / 2.0f, 250.0f);

	UITextList.push_back(titleText);
	UITextList.push_back(enterText);
}

AppManager* AppManager::CreateInstance(sf::RenderWindow* window, int screenWidth, int screenHeight)
{
	instance = new AppManager(window, screenWidth, screenHeight);
	return instance;
}

AppManager* AppManager::GetInstance()
{
	return instance;
}

void AppManager::DeleteInstance()
{
	GameManager::DeleteInstance();
	delete instance;
}

void AppManager::Update()
{
	switch (currentState) {
	case title:
		input.PlayerInput();

		if (input.isKeyReleased(sf::Keyboard::Enter)) {
			StartGame();
		}

		break;
	case mapSelect:
		break;
	case game:
		gameManager->Update();
		break;
	case playerVictory:
		break;
	case enemyVictory:
		break;
	}
}

void AppManager::Draw()
{
	if (currentState == game) {
		gameManager->Draw();
	}
	else {
		for (sf::Text text : UITextList) {
			window->draw(text);
		}
	}
}

void AppManager::SetState(AppState newState)
{
	currentState = newState;
}
