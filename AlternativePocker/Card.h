#pragma once
#include "GlobalTypes.h"

struct Card
{
	card_type _card;
	bool _isSevenClubs;
	char _cardString[4];
};

void SetUpCardDesk(Card*);