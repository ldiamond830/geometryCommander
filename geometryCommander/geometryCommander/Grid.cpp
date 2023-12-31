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
}

//used with loading a grid from path so creates all boxes as empty and then each box's type is set in GameManager.cpp
Grid::Grid(int screenWidth, int screenHeight, int rowSize, int columnSize)
{
	this->gridWidth = rowSize;
	this->gridHeight = columnSize;
	int boxWidth = screenWidth / rowSize;
	int boxHeight = screenHeight / columnSize;
	for (unsigned int i = 0; i < rowSize; i++) {
		gridBoxes.push_back(std::vector<GridBox*>());
		for (unsigned int j = 0; j < columnSize; j++)
		{
			gridBoxes[i].push_back(new GridBox(boxWidth, boxHeight, empty, boxWidth * i, boxHeight * j, sf::Vector2f(i,j)));
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
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
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
	bool complete = false;
	
	std::stack<sf::Vector2f>* scopedPath = new std::stack <sf::Vector2f>();
	while (!complete) {
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
					scopedPath->push(sf::Vector2f(end->GetCenter()));
					//Vertex* currentParent = pathCell->GetParent();
					while (pathCell->index.x != start->index.x || pathCell->index.y != start->index.y) {
						//path.push(new GridBox(*pathCell->GetParent()));
						scopedPath->push(sf::Vector2f(pathCell->GetParent()->GetCenter()));
						pathCell = pathCell->GetParent();
					}
					scopedPath->pop();
					//stores path in gridbox for use when moving a piece
					end->path = scopedPath;
					complete = true;
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
			complete = true;
		}
	}

	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			gridBoxes[x][y]->ResetParent();
		}
	}
}

void Grid::MovePiece(GridBox* start, GridBox* end)
{
	start->occupyingPiece->StartMove(end->path);
	UpdateOccupyingPiece(end, start->occupyingPiece);
	start->occupyingPiece = nullptr;
	start->SetType(gridBoxType::empty);
}

//paths for player pieces are calculated ahead of time to figure out which boxes are in the piece's movement range, so a separate method is needed
void Grid::MoveEnemyPiece(GridBox* start, GridBox* end)
{
	FindPath(start, end);
	MovePiece(start, end);
}


//updates all relevant values when a piece is spawned or moves
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

//this method may be unessesary 
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

			 //checks if values need to be added to the piece's cover map
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
	CalculatePaths(piece, range);
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			//if the length of the path is less than the movement range displays it as in range, if no path is found to a box it's path will be nullptr
			if (box->path != nullptr && box->path->size() <= range) {
				box->SetInRange();
			}
		}
	}
}

void Grid::ClearBoxesInRange()
{
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			if (box->path != nullptr) {
				box->ResetInRange();
			}
			
		}
	}
}

void Grid::CalculatePaths(GamePiece* piece, int range)
{
	for (std::vector<GridBox*> column : gridBoxes)
	{
		for (GridBox* box : column) {
			//checks manhattan distance before path distance to running A* on boxes that would be out of range reguardless of any obstacles along the path
			if (MyUtils::GetInstance()->ManhattanDistance(piece->GetIndex().x, piece->GetIndex().y, box->index.x, box->index.y) <= range && box->GetType() == empty) {
				//calculates the shortest possible path to the current box
				FindPath(gridBoxes[piece->GetIndex().x][piece->GetIndex().y], box);
			}
		}
	}
}
