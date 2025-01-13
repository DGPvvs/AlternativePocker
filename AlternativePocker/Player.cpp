#include <ctime>

#include "Player.h"
#include "GlobalConstants.h"
#include "GamePlay.h"

int Max(int a, int b)
{
	return b > a ? b : a;
}

bool HasSevenClubs(Card* cards)
{
	bool isHasSevenClubs = false;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		isHasSevenClubs = isHasSevenClubs || cards[i]._isSevenClubs;
	}

	return isHasSevenClubs;
}

void GeneratePlayerString(Player& player, Card* cards)
{
	int idx = 0;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		int cardIdx = 0;
		while (cards[i]._cardString[cardIdx] != '\0')
		{
			player._cardsToString[idx++] = cards[i]._cardString[cardIdx++];
		}

		player._cardsToString[idx++] = ' ';
	}

	player._cardsToString[idx] = '\0';
}

int ThreeOfTheSameSuit(Card* cards)
{
	bool result = true;
	int points = 0;

	card_type check = cards[0]._card & Suit::SuitMask;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		result = result && ((cards[i]._card & Suit::SuitMask) == check);
	}

	if (result)
	{
		for (int i = 0; i < CARDS_COUNT; i++)
		{
			points += (cards[i]._card & Rank::RankMask);
		}
	}

	return points;
}

int DifferentCards(Card* cards)
{
	int points = 0;

	if (ThreeOfTheSameSuit(cards) == 0)
	{
		int currentMax = 0;

		for (int i = 0; i < CARDS_COUNT; i++)
		{
			Card card = cards[i];

			if (!card._isSevenClubs && ((card._card & Rank::RankMask) > currentMax))
			{
				currentMax = card._card & Rank::RankMask;
			}
		}

		points = currentMax + HasSevenClubs(cards) ? SEVEN_CLUBS_POINT : 0;
	}

	return points;
}

int TwoCardsOfPip(Card* cards)
{
	int points = 0;
	if (HasSevenClubs(cards))
	{
		for (int i = 0; i < CARDS_COUNT - 1; i++)
		{
			for (int j = 0; j < CARDS_COUNT; j++)
			{
				if ((cards[i]._card & Pip::PipMask) == (cards[j]._card & Pip::PipMask))
				{
					int localPoints = (cards[i]._card & Rank::RankMask);
					points = Max(points, localPoints);
				}
			}
		}

		points = (2 * points) + SEVEN_CLUBS_POINT;
	}

	return points;
}

int TwoCardsOfSuit(Card* cards)
{
	int points = 0;

	if (ThreeOfTheSameSuit(cards) == 0)
	{
		for (int i = 0; i < CARDS_COUNT - 1; i++)
		{
			for (int j = i + 1; j < CARDS_COUNT; j++)
			{
				if ((cards[i]._card & Suit::SuitMask) == (cards[j]._card & Suit::SuitMask))
				{
					int localPoints = (cards[i]._card & Rank::RankMask) + (cards[j]._card & Rank::RankMask);
					points = Max(points, localPoints);
				}
			}
		}

		points = points + (HasSevenClubs(cards) ? SEVEN_CLUBS_POINT : 0);
	}

	return points;
}

int TwoSevens(Card* cards)
{
	int sevenCount = 0;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		sevenCount += ((cards[i]._card & Pip::PipMask) == Pip::N7) ? 1 : 0;
	}

	return sevenCount == 2 ? TWO_SEVEN_POINT : 0;
}

int TwoAces(Card* cards)
{
	int aceCount = 0;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		aceCount += ((cards[i]._card & Pip::PipMask) == Pip::A) ? 1 : 0;
	}

	return aceCount == 2 ? TWO_ACE_POINT : 0;
}

int ThreeOfAKind(Card* cards)
{
	card_type check = cards[0]._card & Pip::PipMask;

	bool result = true;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		result = result && ((cards[i]._card & Pip::PipMask) == check);
	}

	return result ? (3 * (cards[0]._card & RankMask)) : 0;
}

int ThreeSevens(Card* cards)
{
	bool result = true;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		result = result && ((cards[i]._card & Pip::PipMask) == Pip::N7);
	}

	return result ? MAX_POINT : 0;
}

int CalcPoints(Card* cards)
{
	int points = 0;

	points = Max(points, ThreeSevens(cards));
	points = Max(points, ThreeOfAKind(cards));
	points = Max(points, ThreeOfTheSameSuit(cards));
	points = Max(points, TwoAces(cards));
	points = Max(points, TwoSevens(cards));
	points = Max(points, TwoCardsOfSuit(cards));
	points = Max(points, TwoCardsOfPip(cards));
	points = Max(points, DifferentCards(cards));

	return points;
}

void InitEmptyPlayer(Player& player)
{
	player._playerActive = PlayerCondition::Unactive;
	player._chips = 0;
	player._lastRaice = 0;
	player._currentPoints = 0;
}

bool IsPlayerInDeal(player_condition_type condition)
{
	return (condition != PlayerCondition::Unactive) && ((condition & PlayerCondition::Active) == PlayerCondition::Active);
}

int CalcMaxRaise(Player* players)
{
	int maxRaise = INT_MAX;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		player_condition_type condition = players[i]._playerActive;
		int playerChips = players[i]._chips;
		if (IsPlayerInDeal(condition) && playerChips < maxRaise)
		{
			maxRaise = playerChips;
		}
	}

	return maxRaise;
}

void SetCards(Player& player, Card* cardsDesk, int& deckSize)
{
	srand(time(0));
	Card* cards = new Card[CARDS_COUNT];

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		int randomIndex = rand() % deckSize;

		cards[i] = cardsDesk[randomIndex];
		cardsDesk[randomIndex] = cardsDesk[--deckSize];
	}

	player._currentPoints = CalcPoints(cards);
	GeneratePlayerString(player, cards);

	delete[] cards;
	cards = nullptr;
}

