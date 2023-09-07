#include "Grid.h"

Grid::Grid(int screenWidth, int screenHeight, int rowSize, int columnSize)
{
	this->gridWidth = rowSize;
	this->gridHeight = columnSize;

	int boxWidth = screenWidth / rowSize;
	int boxHeight = screenHeight / columnSize;
	for (unsigned int i = 0; i < columnSize; i++) {
		gridBoxes.push_back(std::vector<GridBox*>());
		for (unsigned int j = 0; j < rowSize; j++)
		{
			gridBoxes[i].push_back(new GridBox(boxWidth, boxHeight, empty, boxWidth * j, boxHeight * i));
		}
	}
}

Grid::~Grid()
{
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			delete box;
		}
	}
}

void Grid::Draw(sf::RenderWindow* window)
{
	for(std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			box->Draw(window);
		}
	}
}

void Grid::CalculateHCosts(int endX, int endY)
{
	for (int x = 0; x < gridHeight; x++)
	{
		for (int y = 0; y < gridWidth; y++)
		{
			gridBoxes[x][y]->SetHCost(endX, endY);
		}
	}
}
