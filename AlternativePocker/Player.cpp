#include "Player.h"
#include "GlobalConstants.h"
#include "GamePlay.h"

void InitEmptyPlayer(Player& player)
{
	player._name = "";
	player._cardsAndRangeToString = "";
	player._isHasSevenClubs = SEVEN_CLUBS_NOT_PRESENT;
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

int SetCards(Card* cardsDesk)
{
	srand(time(0));
	this->ClearCards();
	this->_isHasSevenClubs = SEVEN_CLUBS_NOT_PRESENT;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		int randomIndex = rand() % cardsDesk.size();

		Card card = cardsDesk[randomIndex];
		this->_cards.push_back(card);
		this->_isHasSevenClubs = this->_isHasSevenClubs || card.GetIsSevenClubs();
		cardsDesk.erase(cardsDesk.begin() + randomIndex);
	}

	return CalcPoints();
}
