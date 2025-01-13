#pragma once

#include "GlobalTypes.h"
#include "Card.h"

struct Player
{
	player_condition_type _playerActive;
	char _cardsToString[20];

	int _chips;
	int _lastRaice;
	int _currentPoints;
};

void InitEmptyPlayer(Player&);
bool IsPlayerInDeal(player_condition_type);
int CalcMaxRaise(Player*);
void SetCards(Player&, Card*, int&);