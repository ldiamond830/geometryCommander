#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include<memory>
#include <fstream>
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

struct mapData {
	std::string mapName;
	sf::Text* mapText;
};

class AppManager
{
private:
	AppManager(sf::RenderWindow*, int, int);
	~AppManager();
	sf::RenderWindow* window;
	int screenWidth;
	int screenHeight;
	static AppManager* instance;
	sf::Font UIFont;
	AppState currentState;
	GameManager* gameManager;
	void StartGame(std::string map);
	void InitTitle();
	void InitMapSelect();
	void SelectMap(int index);
	std::vector<sf::Text*> UITextList;
	std::vector<mapData> mapList;
	int selectedMapIndex;
	std::vector<std::shared_ptr<Button<AppManager>>> UIButtonList;
	InputManager input;
	void ClearUI();

public:
	static AppManager* CreateInstance(sf::RenderWindow*, int screenWidth, int screenHeight);
	static AppManager* GetInstance();
	static void DeleteInstance();
	void Update();
	void Draw();
	void PlayerWin();
	void PlayerLose();
};

