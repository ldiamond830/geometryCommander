#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include<memory>
#include "GameManager.h"
#include "InputManager.h"
#include "Button.h"
enum AppState {
title,
mapSelect,
game,
playerVictory,
enemyVictory
};

class AppManager
{
private:
	AppManager(sf::RenderWindow*, int, int);
	sf::RenderWindow* window;
	int screenWidth;
	int screenHeight;
	static AppManager* instance;
	sf::Font UIFont;
	AppState currentState;
	GameManager* gameManager;
	void StartGame();
	void InitTitle();
	std::vector<sf::Text> UITextList;
	std::vector<std::shared_ptr<Button<AppManager>>> UIButtonList;
	InputManager input;
public:
	static AppManager* CreateInstance(sf::RenderWindow*, int screenWidth, int screenHeight);
	static AppManager* GetInstance();
	static void DeleteInstance();
	void Update();
	void Draw();
	void SetState(AppState);
};

