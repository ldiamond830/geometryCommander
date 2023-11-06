#pragma once
#include "PlayerPiece.h"
//type of player piece that has less attack range but more maximum damage
class AssaultPlayerPiece : public PlayerPiece
{
public:
	AssaultPlayerPiece(int xPos, int yPos);
};

