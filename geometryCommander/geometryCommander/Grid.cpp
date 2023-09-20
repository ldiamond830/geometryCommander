#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include <stack>

Grid::Grid() : Grid(800, 600, 10, 10)
{
}

Grid::Grid(int screenWidth, int screenHeight, int rowSize, int columnSize)
{
	this->gridWidth = rowSize;
	this->gridHeight = columnSize;
	pathMatchesInput = false;
	int boxWidth = screenWidth / rowSize;
	int boxHeight = screenHeight / columnSize;
	for (unsigned int i = 0; i < rowSize; i++) {
		gridBoxes.push_back(std::vector<GridBox*>());
		for (unsigned int j = 0; j < columnSize; j++)
		{
			gridBoxes[i].push_back(new GridBox(boxWidth, boxHeight, empty, boxWidth * i, boxHeight * j, sf::Vector2f(i,j)));
		}
	}
	//gridBoxes[0][4]->SetType(fullCover);
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
			gridBoxes[x][y]->ResetParent();
		}
	}
}

void Grid::FindPath(GridBox* start, GridBox* end)
{
	CalculateHCosts(end->index.x, end->index.y);
	start->SetGCost(0);
	std::vector<GridBox*> openList;
	std::vector<GridBox*> closeList;

	closeList.push_back(start);
	GridBox* currentCell = start;
	pathMatchesInput = false;
	if (!path.empty()) {
		for (int i = 0; i < path.size(); i++) {
			path.pop();
		}
	}
	

	while (!pathMatchesInput) {
		for (int x = currentCell->index.x - 1; x <= currentCell->index.x + 1; x++) {
			for (int y = currentCell->index.y - 1; y <= currentCell->index.y + 1; y++) {

				//skip current cell
				if (x == currentCell->index.x && y == currentCell->index.y) {
					continue;
				}

				//skips diagonals
				if ((x == currentCell->index.x - 1 && y == currentCell->index.y - 1) ||
					(x == currentCell->index.x - 1 && y == currentCell->index.y + 1) ||
					(x == currentCell->index.x + 1 && y == currentCell->index.y - 1) ||
					(x == currentCell->index.x + 1 && y == currentCell->index.y + 1)) {
					continue;
				}

				//Don't check out of bounds of the grid.
				if (x < 0 || x > gridWidth - 1 || y < 0 || y > gridHeight - 1) {
					continue;
				}

				if (gridBoxes[x][y]->GetType() == fullCover || gridBoxes[x][y]->GetType() == occupied) {
					continue;
				}


				//check if the cell has already been calculated
				for (unsigned int k = 0; k < closeList.size(); k++) {
					if (closeList[k]->xPos == gridBoxes[x][y]->xPos && closeList[k]->yPos == gridBoxes[x][y]->yPos) {
						continue;
					}
				}

				if (gridBoxes[x][y]->xPos == end->xPos && gridBoxes[x][y]->yPos == end->yPos) {
					end->SetParentIfCheaper(new GridBox(*currentCell));
					closeList.push_back(end);

					GridBox* pathCell = end;
					path.push(end);
					//Vertex* currentParent = pathCell->GetParent();
					while (pathCell->index.x != start->index.x || pathCell->index.y != start->index.y) {
						path.push(pathCell->GetParent());
						pathCell = pathCell->GetParent();
					}
					path.pop();
					pathMatchesInput = true;
				}

				GridBox* nextCell = gridBoxes[x][y];
				if (gridBoxes[x][y]->SetParentIfCheaper(new GridBox(*currentCell))) {

					bool doesntContainCell = true;
					for (int i = 0; i < openList.size(); i++) {
						if (openList[i]->xPos == gridBoxes[x][y]->xPos && openList[i]->yPos == gridBoxes[x][y]->yPos) {
							doesntContainCell = false;
						}
					}

					if (doesntContainCell) {
						openList.push_back(gridBoxes[x][y]);
					}

				}
			}
		}

		if (openList.size() != 0) {
			int shortestDist = openList[0]->GetFCost();
			int shortestIndex = 0;
			for (int i = 0; i < openList.size(); i++) {
				if (openList[i]->GetFCost() < shortestDist) {
					shortestDist = openList[i]->GetFCost();
					shortestIndex = i;
				}
			}

			currentCell = openList[shortestIndex];
			closeList.push_back(currentCell);
			openList.erase(openList.begin() + shortestIndex);
		}
		else {
			//no path found
			pathMatchesInput = true;
		}
	}
}

void Grid::MovePiece(GridBox* start, GridBox* end)
{
	FindPath(start, end);
	while (!path.empty()) 
	{
		start->occupyingPiece->MoveToNext(path.top()->GetCenter());
		path.pop();
	}
	end->occupyingPiece = start->occupyingPiece;
	start->occupyingPiece = nullptr;
	start->SetType(gridBoxType::empty);
	end->SetType(gridBoxType::occupied);
}

GridBox* Grid::GetBoxFromPosition(sf::Vector2i position)
{
	for (std::vector<GridBox*> boxList : gridBoxes) {
		for (GridBox* box : boxList) {
			//check if position is to the right of current box
			if (box->GetCenter().x + box->GetWidth() / 2 < position.x)
				continue;
			//check if position is to the left of current box
			if (box->GetCenter().x - box->GetWidth() / 2 >= position.x)
				continue;
			//check if position is above current box
			if (box->GetCenter().y + box->GetHeight() / 2 < position.y)
				continue;
			if (box->GetCenter().y - box->GetHeight() / 2 >= position.y)
				continue;

			return box;
		}
	}
	return nullptr;
}
