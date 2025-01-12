#pragma once
#include "GlobalTypes.h"
#include <string>

struct Card
{
    card_type _card;
    bool _isSevenClubs;
    std::string _cardString;
};

void SetUpCardDesk(Card*);
std::string CardToString(card_type);