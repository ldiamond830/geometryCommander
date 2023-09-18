#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include "GridBox.h"
class Grid
{
private:
	int gridWidth;
	int gridHeight;
	bool pathMatchesInput;
	std::stack<GridBox*> path;
public:
		std::vector<std::vector<GridBox*>> gridBoxes;
		Grid(int screenWidth, int screenHeight, int rowSize, int columnSize);
		~Grid();
		void Draw(sf::RenderWindow* window);
		void CalculateHCosts(int, int);
		void FindPath(GridBox* start, GridBox* end);
		void MovePiece(GridBox*, GridBox*);
};

