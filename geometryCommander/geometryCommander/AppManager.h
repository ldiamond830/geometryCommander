#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum AppState {
title,
mapSelect,
game,
playerWon,
enemyWon
};

class AppManager
{
private:
	AppManager(sf::RenderWindow*, int, int);
	AppManager* instance;
	sf::Font UIFont;

public:
	static AppManager* CreateInstance(sf::RenderWindow*, int screenWidth, int screenHeight);
	static AppManager GetInstance();
	void Update();
};

