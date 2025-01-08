#pragma once
#include <string>
#include "GlobalTypes.h"

struct Player
{
	std::string _name;
	bool _isHasSevenClubs;
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
int SetCards(Card*);
