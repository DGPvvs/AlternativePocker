#pragma once

#include "Player.h"
#include "GlobalEnums.h"

struct Deal
{
	int _pot;
	int _lastGameRaise;
	int _currentMaxRaise;
	bool _dealFlag;
};

GameCondition DealLoop(Player*, Deal*);
bool DealStart(Player*, Deal*);
void DealPlay(Player*, Deal*);
void DeterminingWinner(Player*, Deal*);
void ClearDeal(Deal*);
