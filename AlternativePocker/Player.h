#pragma once
#include <string>

#include "GlobalTypes.h"
#include "Card.h"

struct Player
{
	std::string _name;
	player_condition_type _playerActive;
	std::string _cardsAndRangeToString;

	int _chips;
	int _lastRaice;
	int _currentPoints;
	int _id;
};

void InitEmptyPlayer(Player&);
bool IsPlayerInDeal(player_condition_type);
int CalcMaxRaise(Player*);
void SetCards(Player&, Card*, int&);
