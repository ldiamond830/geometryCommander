#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "GamePiece.h"

enum gridBoxType 
{
	empty,
	occupied,
	halfCover,
	fullCover
};
class GridBox
{
	public:
	GridBox(int width, int height, gridBoxType type, float _xPos, float _yPos, sf::Vector2f _index);
	GridBox(const GridBox& other);
	~GridBox();
	void SetType(gridBoxType newType);
	void Draw(sf::RenderWindow* window);
	void SetHCost(int endX, int endY);
	int GetFCost(); 
	int GetGCost();
	int GetHCost();
	GridBox* GetParent();
	bool SetParentIfCheaper(GridBox* possibleParent);
	float xPos;
	float yPos;
	GamePiece* occupyingPiece = nullptr;
	sf::Vector2f GetCenter();
	gridBoxType GetType();
	sf::Vector2f index;
	int GetWidth();
	int GetHeight();
	void ResetParent();
	void SetGCost(int);
	void SetOccupyingPiece(GamePiece* piece);
	bool inPlayerMoveRange;
	std::stack<sf::Vector2f>* path = nullptr;
	void SetInRange();
	void ResetInRange();

private:
	sf::RectangleShape* visual;
	gridBoxType type;
	int height;
	int width;
	int gCost;
	void SetTypeValues();
	int hCost = 0;
	GridBox* parent = nullptr;
	
};

