#pragma once
#include "GlobalTypes.h"
#include <string>

struct Card
{
private:
    card_type _card;
    bool _isSevenClubs;
    std::string _catdStrong;
};

void SetUpCardDesk(Card*);
