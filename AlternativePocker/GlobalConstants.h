#pragma once
#include <string>
#include "GlobalTypes.h"
#include "GlobalEnums.h"

const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 9;

const std::string  WARNING = "Enter correct data!";
const std::string  FILE_NAME = "pockergame.txt";

const bool DEAL_PLAY = true;
const bool NOT_DEAL_PLAY = false;

const bool FIRST_DEAL = true;
const bool CONTINUE_DEAL = false;

const bool SEVEN_CLUBS_PRESENT = true;
const bool SEVEN_CLUBS_NOT_PRESENT = false;


const int CHIP_VALUE = 10;
const int START_POINTS = 100;
const int CHIPS_ADD_VALUE = 50;

const int PIPS_ROW = 2;
const int CARDS_COL = 8;
const int SUITS = 4;

const int CARDS_COUNT = 3;
const int DECK_COUNT = CARDS_COL * SUITS;

const int MAX_POINT = 34;
const int TWO_SEVEN_POINT = 23;
const int TWO_ACE_POINT = 22;
const int SEVEN_CLUBS_POINT = 11;

const card_type pipsMatrix[PIPS_ROW][CARDS_COL] =
{
    {Pip::N7, Pip::N8, Pip::N9, Pip::N10, Pip::J, Pip::Q, Pip::K, Pip::A },
    {Rank::Seven, Rank::Eight, Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen, Rank::King, Rank::Ace}
};

const card_type suits[SUITS] = { Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades };
