#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <map>
#include "GridBox.h"
class Grid
{
private:
	int gridWidth;
	int gridHeight;
	bool pathMatchesInput;
	//std::stack<GridBox*> path;
	std::stack<sf::Vector2f*>* testPath;
public:
		std::vector<std::vector<GridBox*>> gridBoxes;
		Grid();
		Grid(int screenWidth, int screenHeight, int rowSize, int columnSize);
		~Grid();
		void Draw(sf::RenderWindow* window);
		void CalculateHCosts(int, int);
		void FindPath(GridBox* start, GridBox* end);
		void MovePiece(GridBox*, GridBox*);
		//separate from move so it can be called when pieces are spawned, not just moved
		void UpdateOccupyingPiece(GridBox* box, GamePiece* piece);
		GridBox* GetBoxFromPosition(sf::Vector2i);
		GridBox* GetBoxFromOccupyingPiece(GamePiece* piece);
		int GetWidth();
		int GetHeight();
		std::map<coverDirection, int> GetCoverAtPosition(GridBox* box);

};

