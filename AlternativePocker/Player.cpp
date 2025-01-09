#include <ctime>

#include "Player.h"
#include "GlobalConstants.h"
#include "GamePlay.h"

void InitEmptyPlayer(Player& player)
{
	player._name = "";
	player._cardsAndRangeToString = "";
	player._playerActive = PlayerCondition::Unactive;
	player._chips = 0;
	player._lastRaice = 0;
	player._currentPoints = 0;
	player._id = 0;
}

bool IsPlayerInDeal(player_condition_type condition)
{
	return (condition != PlayerCondition::Unactive) && ((condition & PlayerCondition::Fold) != PlayerCondition::Fold);
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

void SetCards(Player& player,  Card* cardsDesk, int& deckSize)
{
	srand(time(0));
	Card* cards = new Card[CARDS_COUNT];

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		int randomIndex = rand() % deckSize;

		cards[i] = cardsDesk[randomIndex];
		deckSize--;
		cardsDesk[randomIndex] = cardsDesk[deckSize];
	}

	CalcPoints();

	delete[] cards;
	cards = nullptr;
}
