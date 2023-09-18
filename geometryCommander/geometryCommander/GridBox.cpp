
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "GridBox.h"
GridBox::GridBox(int _width, int _height, gridBoxType _type, float _xPos, float _yPos, sf::Vector2f _index)
{
	width = _width;
	height = _height;
	xPos = _xPos;
	yPos = _yPos;
	type = _type;
	index = _index;
	visual = new sf::RectangleShape(sf::Vector2f(width, height));
	visual->setOutlineThickness(2);
	visual->setOutlineColor(sf::Color(255,255,255));
	visual->setPosition(sf::Vector2f(xPos, yPos));
	SetTypeValues();
}

GridBox::GridBox(const GridBox& other)
{
	width = other.width;
	height = other.height;
	xPos = other.xPos;
	yPos = other.yPos;
	type = other.type;
	index = other.index;
	parent = other.parent;
	visual = new sf::RectangleShape(sf::Vector2f(width, height));
	visual->setOutlineThickness(2);
	visual->setOutlineColor(sf::Color(255, 255, 255));
	visual->setPosition(sf::Vector2f(xPos, yPos));
	SetTypeValues();
}

GridBox::~GridBox()
{
	delete visual;
}

void GridBox::SetType(gridBoxType newType)
{
	type = newType;
	SetTypeValues();
}

void GridBox::Draw(sf::RenderWindow* window)
{
	window->draw(*visual);
}

void GridBox::SetHCost(int endX, int endY)
{
	float x = abs(index.x - endX);
	float y = abs(index.y - endY);
	hCost =  x + y;
}

int GridBox::GetFCost()
{
	return moveCost + hCost;
}

int GridBox::GetGCost()
{
	return moveCost;
}

int GridBox::GetHCost()
{
	return hCost;
}

GridBox* GridBox::GetParent()
{
	return parent;
}

bool GridBox::SetParentIfCheaper(GridBox* possibleParent)
{
	int xDist = index.x - possibleParent->index.x;
	int yDist = index.y - possibleParent->index.y;

	float moveCost = possibleParent->GetGCost() + sqrt(pow(xDist, 2) + pow(yDist, 2));
	if (parent == nullptr || moveCost < GetGCost()) {
		this->moveCost = moveCost;
		parent = possibleParent;
		return true;

	}


	return false;
}

sf::Vector2f GridBox::GetCenter()
{
	return sf::Vector2f(xPos + (width/2), yPos + (height/2));
}

gridBoxType GridBox::GetType()
{
	return type;
}

void GridBox::SetTypeValues()
{
	switch (type) {
	case empty:
		moveCost = 1;
		visual->setFillColor(sf::Color(0, 0, 0));
		break;
	case occupied:
		moveCost = INT_MAX;
		visual->setFillColor(sf::Color(0, 0, 0));
		break;
	case halfCover:
		moveCost = 3;
		visual->setFillColor(sf::Color(128, 128, 128));
		break;
	case fullCover:
		moveCost = INT_MAX;
		visual->setFillColor(sf::Color(255, 255, 255));
		break;
	}
}
