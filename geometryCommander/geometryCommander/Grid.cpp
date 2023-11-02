#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include <stack>
#include "MyUtils.h"
Grid::Grid() : Grid(800, 600, 10, 10)
{
	//defualt map 
	gridBoxes[0][1]->SetType(halfCover);
	gridBoxes[2][1]->SetType(halfCover);
	gridBoxes[4][1]->SetType(halfCover);
	gridBoxes[6][1]->SetType(halfCover);
	gridBoxes[8][1]->SetType(halfCover);

	gridBoxes[1][7]->SetType(halfCover);
	gridBoxes[3][7]->SetType(halfCover);
	gridBoxes[5][7]->SetType(halfCover);
	gridBoxes[7][7]->SetType(halfCover);

	gridBoxes[3][3]->SetType(fullCover);
	gridBoxes[2][4]->SetType(fullCover);
	gridBoxes[3][4]->SetType(fullCover);
	gridBoxes[4][4]->SetType(fullCover);
	gridBoxes[3][5]->SetType(fullCover);

	gridBoxes[7][3]->SetType(fullCover);
	gridBoxes[6][4]->SetType(fullCover);
	gridBoxes[7][4]->SetType(fullCover);
	gridBoxes[8][4]->SetType(fullCover);
	gridBoxes[7][5]->SetType(fullCover);

	gridBoxes[1][5]->SetType(fullCover);
	gridBoxes[0][7]->SetType(halfCover);
	
	path = new std::stack <sf::Vector2f>();
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
	
	path = new std::stack <sf::Vector2f>();
}


Grid::~Grid()
{
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			delete box;
		}
	}
	delete path;
}

void Grid::Draw(sf::RenderWindow* window)
{
	for(std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			box->Draw(window);
		}
	}

	for (sf::RectangleShape box : boxesInRange) {
		window->draw(box);
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

void Grid::FindPath(GridBox* start, GridBox* end)
{
	CalculateHCosts(end->index.x, end->index.y);
	start->SetGCost(0);
	std::vector<GridBox*> openList;
	std::vector<GridBox*> closeList;

	closeList.push_back(start);
	GridBox* currentCell = start;
	pathMatchesInput = false;
	if (!path->empty()) {
		for (int i = 0; i < path->size(); i++) {
			path->pop();
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
					//path.push(new GridBox(*end));
					path->push(sf::Vector2f(end->GetCenter()));
					//Vertex* currentParent = pathCell->GetParent();
					while (pathCell->index.x != start->index.x || pathCell->index.y != start->index.y) {
						//path.push(new GridBox(*pathCell->GetParent()));
						path->push(sf::Vector2f(pathCell->GetParent()->GetCenter()));
						pathCell = pathCell->GetParent();
					}
					path->pop();
					//stores path in gridbox for use when moving a piece
					end->path = path;
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
	for (int x = 0; x < gridHeight; x++)
	{
		for (int y = 0; y < gridWidth; y++)
		{
			gridBoxes[x][y]->ResetParent();
		}
	}
}

void Grid::MovePiece(GridBox* start, GridBox* end)
{
	FindPath(start, end);
	/*
	std::queue<sf::Vector2f> temp;
	while (!path.empty()) 
	{
		temp.push(path.top()->GetCenter());
		//this may be causing a memory leak check back later
		path.pop();
	}
	*/
	start->occupyingPiece->StartMove(end->path);
	UpdateOccupyingPiece(end, start->occupyingPiece);
	start->occupyingPiece = nullptr;
	start->SetType(gridBoxType::empty);
}

void Grid::UpdateOccupyingPiece(GridBox* box, GamePiece* piece)
{
	box->SetOccupyingPiece(piece);
	box->SetType(gridBoxType::occupied);

	piece->coverMap.clear();
	piece->coverMap = std::map<coverDirection, int>(GetCoverAtPosition(box));
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

GridBox* Grid::GetBoxFromOccupyingPiece(GamePiece* piece)
{
	for (std::vector<GridBox*> boxList : gridBoxes)
	{
		for (GridBox* box : boxList) {
			if (box->occupyingPiece == piece) {
				return box;
			}
		}
	}
	//error case
	return nullptr;
}

int Grid::GetWidth()
{
	return gridWidth;
}

int Grid::GetHeight()
{
	return gridHeight;
}

std::map<coverDirection, int> Grid::GetCoverAtPosition(GridBox* box)
{
	 std::map<coverDirection, int> mapToReturn = std::map<coverDirection, int>();
	 for (int x = box->index.x - 1; x <= box->index.x + 1; x++) {
		 for (int y = box->index.y - 1; y <= box->index.y + 1; y++) {

			 //skip current cell
			 if (x == box->index.x && y == box->index.y) {
				 continue;
			 }

			 //skips diagonals
			 if ((x == box->index.x - 1 && y == box->index.y - 1) ||
				 (x == box->index.x - 1 && y == box->index.y + 1) ||
				 (x == box->index.x + 1 && y == box->index.y - 1) ||
				 (x == box->index.x + 1 && y == box->index.y + 1)) {
				 continue;
			 }

			 //Don't check out of bounds of the grid.
			 if (x < 0 || x > gridWidth - 1 || y < 0 || y > gridHeight - 1) {
				 continue;
			 }

			 if (gridBoxes[x][y]->GetType() == halfCover) {
				 if (x > box->index.x) {
					 mapToReturn.emplace(RIGHT, 25);
				 }
				 else if (x < box->index.x) {
					 mapToReturn.emplace(LEFT, 25);
				 }
				 else if (y > box->index.y) {
					 mapToReturn.emplace(DOWN, 25);
				 }
				 else {
					mapToReturn.emplace(UP, 25);
				 }
			 }
			 else if (gridBoxes[x][y]->GetType() == fullCover) {
				 if (x > box->index.x) {
					 mapToReturn.emplace(RIGHT, 50);
				 }
				 else if (x < box->index.x) {
					 mapToReturn.emplace(LEFT, 50);
				 }
				 else if (y > box->index.y) {
					 mapToReturn.emplace(DOWN, 50);
				 }
				 else {
					 mapToReturn.emplace(UP, 50);
				 }
			 }

		 }
	 }
	 return mapToReturn;
}

void Grid::ShowBoxesInRange(GamePiece* piece, float range)
{
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			
			box->inPlayerMoveRange = false;
			if (MyUtils::GetInstance()->ManhattanDistance(piece->GetIndex().x, piece->GetIndex().y, box->index.x, box->index.y) <= range && box->GetType() == empty) {
				box->inPlayerMoveRange = true;
				/*
				sf::RectangleShape newRect = sf::RectangleShape(sf::Vector2f(box->GetWidth(), box->GetHeight()));
				newRect.setPosition(sf::Vector2f(box->xPos, box->yPos));
				newRect.setFillColor(sf::Color(0, 0, 255, 100));
				boxesInRange.push_back(newRect);
				*/
			}
		}
	}
}

void Grid::ClearBoxesInRange()
{
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
				box->inPlayerMoveRange = false;
		}
	}

	boxesInRange.clear();
}
