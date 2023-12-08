#include "AppManager.h"
#include <iostream>
using namespace std;
AppManager* AppManager::instance = nullptr;

AppManager::AppManager(sf::RenderWindow* _window, int _screenWidth, int _screenHeight)
{
	window = _window;
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	currentState = title;
	input = InputManager();
	if (!UIFont.loadFromFile("arial.ttf")) {
		cout << "Error loading font";
	}

	InitTitle();
}

AppManager::~AppManager()
{
	for (sf::Text* text : UITextList) {
		delete text;
	}
}


void AppManager::StartGame(string map)
{
	if (map == "default") {
		gameManager = GameManager::CreateInstance(window, screenWidth, screenHeight, 10, 10, 4, 4);
	}
	else {
		string path = "maps/" + map + ".txt";
		gameManager = GameManager::CreateInstance(window, screenWidth, screenHeight, path);
	}
	currentState = game;
}

void AppManager::InitTitle()
{
	sf::Text* titleText = new sf::Text();
	titleText->setFont(UIFont);
	titleText->setString("GEOMETRY COMMANDER");
	titleText->setCharacterSize(50);
	titleText->setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(titleText);
	titleText->setPosition(screenWidth / 2.0f, 50.0f);

	sf::Text* enterText = new sf::Text();
	enterText->setFont(UIFont);
	enterText->setString("PRESS ENTER");
	enterText->setCharacterSize(30);
	enterText->setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(enterText);
	enterText->setPosition(screenWidth / 2.0f, 250.0f);

	UITextList.push_back(titleText);
	UITextList.push_back(enterText);

	//Button<AppManager> testButton = Button<AppManager>(sf::Vector2f(screenWidth/2.0f, 100), sf::Vector2f(20.0f,20.0f), enterText, this, &AppManager::StartGame);
	//UIButtonList.push_back(std::shared_ptr<Button<AppManager>>(testButton));
}

void AppManager::InitMapSelect()
{
	ClearUI();

	selectedMapIndex = 0;


	sf::Text* controlText = new sf::Text();
	controlText->setFont(UIFont);
	controlText->setString("USE UP AND DOWN ARROW KEYS TO MOVE \n\tAND ENTER TO SELECT");
	controlText->setCharacterSize(30);
	controlText->setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(controlText);
	controlText->setPosition(screenWidth / 2.0f, 50.0f);

	UITextList.push_back(controlText);

	//reads in list of maps from file
	ifstream fileReader("maps/mapList.txt");

	int iterator = 1;
	while (fileReader.good()) {
		string mapName;
		getline(fileReader, mapName);

		sf::Text* mapText = new sf::Text();
		mapText->setFont(UIFont);
		mapText->setString(mapName);
		mapText->setCharacterSize(20);
		mapText->setFillColor(sf::Color::Red);
		MyUtils::GetInstance()->CenterText(mapText);
		mapText->setPosition(screenWidth / 2.0f, 50.0f + (80 * iterator));
		UITextList.push_back(mapText);
		mapList.push_back(mapData{mapName = mapName, mapText = mapText});
		iterator++;
	}


	currentState = mapSelect;
	SelectMap(selectedMapIndex);
}

void AppManager::SelectMap(int index)
{
	if (index < 0) {
		index = mapList.size() -1;
	}

	//reset previous selection
	mapList[selectedMapIndex].mapText->setFillColor(sf::Color::Red);

	selectedMapIndex = index;
	//set new selection
	mapList[selectedMapIndex].mapText->setFillColor(sf::Color::White);
}

void AppManager::ClearUI()
{
	for (sf::Text* text : UITextList) {
		delete text;
	}
	UITextList.clear();

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
	//FSM
	switch (currentState) {
	case title:
		input.PlayerInput();

		if (input.isKeyReleased(sf::Keyboard::Enter)) {
			InitMapSelect();
		}
		
		break;
	case mapSelect:
		input.PlayerInput();

		if (input.isKeyReleased(sf::Keyboard::Enter)) {
			StartGame(mapList[selectedMapIndex].mapName);
		}
		else if (input.isKeyReleased(sf::Keyboard::Up)) {
			//wrapping wasn't working properly so I hard coded
			if (selectedMapIndex == 0) {
				SelectMap(mapList.size() - 1);
			}
			else {
				SelectMap((selectedMapIndex - 1) % mapList.size());
			}

		}
		else if (input.isKeyReleased(sf::Keyboard::Down)) {
			SelectMap((selectedMapIndex + 1) % mapList.size());
		}
		break;
	case game:
		gameManager->Update();
		break;
	case playerVictory:
		cout << "pee urine even";
		break;
	case enemyVictory:
		cout << "poo shit even";
		break;
	}
}

void AppManager::Draw()
{
	if (currentState == game) {
		gameManager->Draw();
	}
	else {
		for (sf::Text* text : UITextList) {
			window->draw(*text);
		}
		/*
		for (Button button : UIButtonList) {
			button.Draw(window);
		}
		*/
	}
}

void AppManager::PlayerWin()
{
	ClearUI();
	currentState = playerVictory;

	sf::Text* winText = new sf::Text();
	winText->setFont(UIFont);
	winText->setString("YOU WIN");
	winText->setCharacterSize(50);
	winText->setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(winText);
	winText->setPosition(screenWidth / 2.0f, 50.0f);
	UITextList.push_back(winText);

}

void AppManager::PlayerLose()
{
	ClearUI();
	currentState = enemyVictory;

	sf::Text* winText = new sf::Text();
	winText->setFont(UIFont);
	winText->setString("YOU LOSE");
	winText->setCharacterSize(50);
	winText->setFillColor(sf::Color::Red);
	MyUtils::GetInstance()->CenterText(winText);
	winText->setPosition(screenWidth / 2.0f, 50.0f);
	UITextList.push_back(winText);
}

