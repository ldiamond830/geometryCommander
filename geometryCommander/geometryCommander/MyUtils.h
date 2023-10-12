#pragma once
#include <map>
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
	//float naiveLerp
};

