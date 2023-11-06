#pragma once
#include "PlayerPiece.h"
//type of player piece with increased heath and decreased movement range
class HeavyPlayerPiece : public PlayerPiece
{
public:
	HeavyPlayerPiece(int xPos, int yPos);
};

