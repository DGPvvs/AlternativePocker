#pragma once
#include "GlobalTypes.h"

enum PlayerCondition
{
    Unactive = (player_condition_type)0,
    Active = (player_condition_type)0b00000001,
    Fold = Active << 1,
    Call = Fold << 1,
    Raise = Call << 1,
    Hold = Raise << 1
};

enum GameCondition
{
    Win,
    Continue,
    DealEnd,
    End
};

enum FileCondition
{
    OK,
    Error
};

enum Pip
{
    N7 = 0b100000000,
    N8 = N7 << 1,
    N9 = N8 << 1,
    N10 = N9 << 1,
    J = N10 << 1,
    Q = J << 1,
    K = Q << 1,
    A = K << 1,
    PipMask = 0b1111111100000000
};

enum Suit
{
    Clubs = 0b10000,
    Diamonds = Clubs << 1,
    Hearts = Diamonds << 1,
    Spades = Hearts << 1,
    SuitMask = 0b11110000
};

enum Rank
{
    Ace = 11,
    King = 10,
    Queen = 10,
    Jack = 10,
    Ten = 10,
    Nine = 9,
    Eight = 8,
    Seven = 7,
    RankMask = 0b1111,
    NegativeRankMask = ~RankMask
};