#pragma once
#include <string>

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

//int TwoCardsOfPip(Card*);
//int TwoCardsOfSuit(Card*);
//int TwoSevens(Card*);
//int TwoAces(Card*);
//int ThreeOfTheSameSuit(Card*);
//int ThreeOfAKind(Card*);
//int ThreeSevens(Card*);
//int Max(int, int);
//int CalcPoints(Card*);
void InitEmptyPlayer(Player&);
bool IsPlayerInDeal(player_condition_type);
int CalcMaxRaise(Player*);
void SetCards(Player&, Card*, int&);