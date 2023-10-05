#pragma once
#include <map>

class MyUtils
{
private:
		MyUtils();
		static MyUtils* instance;

public:
	static MyUtils* GetInstance();
	~MyUtils();
	int ManhattanDistance(int startX, int startY, int endX, int endY);
};

