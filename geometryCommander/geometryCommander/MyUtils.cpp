#include "MyUtils.h"
MyUtils* MyUtils::instance = nullptr;

MyUtils::MyUtils() {

}

MyUtils* MyUtils::GetInstance()
{
    if (instance == nullptr) {
        instance = new MyUtils();
    }

    return instance;
}

MyUtils::~MyUtils()
{
    delete instance;
}

int MyUtils::ManhattanDistance(int startX, int startY, int endX, int endY)
{
    float x = abs(startX - endX);
    float y = abs(startY - endY);
    return x + y;
}

int MyUtils::ManhattanDistance(GridBox* start, GridBox* end)
{
    return ManhattanDistance(start->index.x, start->index.y, end->index.x, end->index.y);
}
