#pragma once
#include <vector>
#include "GridBox.h"
class Grid
{
private:
	int gridWidth;
	int gridHeight;
	bool pathMatchesInput;
public:
		std::vector<std::vector<GridBox*>> gridBoxes;
		Grid(int screenWidth, int screenHeight, int rowSize, int columnSize);
		~Grid();
		void Draw(sf::RenderWindow* window);
		void CalculateHCosts(int, int);
		void FindPath(GridBox* start, GridBox* end);
};

