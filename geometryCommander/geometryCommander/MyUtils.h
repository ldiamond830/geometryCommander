#pragma once
#include <map>
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "GridBox.h"
class MyUtils
{
private:
		MyUtils();
		static MyUtils* instance;

public:
	static MyUtils* GetInstance();
	~MyUtils();
	int ManhattanDistance(int startX, int startY, int endX, int endY);
	int ManhattanDistance(GridBox* start, GridBox* end);
	void CenterText(sf::Text* text);
};

