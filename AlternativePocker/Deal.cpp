#include "Deal.h"
#include "GlobalConstants.h"
#include "GamePlay.h"
#include <iostream>

GameCondition DealLoop(Player* players, Deal* deal)
{
	int activeCount = ActivePlayersCount(players);

	if (activeCount == 1)
	{
		return GameCondition::DealEnd;
	}

	if (DealStart(players, deal))
	{
		DealPlay(players, deal);
	}

	DeterminingWinner(players);

	deal->_dealFlag = CONTINUE_DEAL;

	return DealLoop(players, deal);
}

bool DealStart(Player* players, Deal* deal)
{
	ClearDeal(deal);
	Card* cardDeck = new Card[DECK_COUNT];

	SetUpCardDesk(cardDeck);

	bool result = DEAL_PLAY;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player._playerActive;

		if (IsPlayerInDeal(condition))
		{
			player._chips -= CHIP_VALUE;
			deal->_pot += CHIP_VALUE;

			result = result && ((player._chips > 0) ? DEAL_PLAY : NOT_DEAL_PLAY);

			std::cout << player._name << ": " << player._chips << std::endl << std::endl;

			player._currentPoints = SetCards(cardDeck);
		}
	}
	delete[] cardDeck;
	cardDeck = nullptr;

	return result;
}

void DealPlay(Player* players, Deal* deal)
{
	int currentCall = 0;
	int currentPlayerIndex = 0;
	bool isFirst = true;
	int activePlayers = ActivePlayersCount(players);

	while ((activePlayers > 1) && currentCall < activePlayers - 1)
	{
		int currentMaxRaise = CalcMaxRaise(players);

		Player& player = players[currentPlayerIndex];

		std::cout << "Pot: " << deal->_pot << std::endl << std::endl;

		bool isCorrect = false;

		while (!isCorrect)
		{
			player_condition_type ChoiceMade = PlayerCondition::Unactive;
			std::cout << "You have given: " << player._lastRaice << std::endl;
			std::cout << "Last raise is: " << deal->_lastGameRaise << std::endl << std::endl;
			std::cout << player._cardsAndRangeToString << std::endl;
			if (isFirst)
			{
				std::cout << player.GetName() << " raise or fold? r/f: ";
				ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold;
			}
			else if (this->_currentMaxRaise <= this->_lastGameRaise)
			{
				std::cout << player.GetName() << " call or fold? c/f: ";
				ChoiceMade = PlayerCondition::Fold | PlayerCondition::Call;
			}
			else
			{
				std::cout << player.GetName() << " raise, call or fold? r/c/f: ";
				ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold | PlayerCondition::Call;
			}

			std::string s;
			std::getline(std::cin, s);
			std::cout << std::endl;

			if ((s == "f" || s == "F") && (ChoiceMade & PlayerCondition::Fold) == PlayerCondition::Fold)
			{
				auto flag = player.GetPlayerCondition();
				flag = flag & (~PlayerCondition::Active);
				flag = flag | PlayerCondition::Fold;
				player.SetPlayerActive(flag);
				isCorrect = true;
			}
			else if ((s == "c" || s == "C") && !isFirst && (ChoiceMade & PlayerCondition::Call) == PlayerCondition::Call)
			{
				this->_playersQu.push(&player);
				currentCall++;
				int lastPlayerRaise = player.GetLastRaise();
				int pays = this->_lastGameRaise - lastPlayerRaise;

				player.SetLastRaise(this->_lastGameRaise);

				player.AddChips(-pays);
				this->_pot += pays;
				isCorrect = true;
			}
			else if ((s == "r" || s == "R") && (ChoiceMade & PlayerCondition::Raise) == PlayerCondition::Raise)
			{
				this->_playersQu.push(&player);

				bool isCorrectPay = false;
				int paymentAmount;

				while (!isCorrectPay)
				{
					std::cout << player.GetName() << " pay: (" << this->_lastGameRaise + 10 << " - " << this->_currentMaxRaise << "): ";
					std::getline(std::cin, s);
					std::cout << std::endl;

					try
					{
						paymentAmount = stoi(s);
						if ((paymentAmount >= this->_lastGameRaise + 10) && paymentAmount <= this->_currentMaxRaise)
						{
							isCorrectPay = true;
						}
					}
					catch (const std::exception&)
					{
						isCorrectPay = false;
					}
				}

				isFirst = false;

				player.SetLastRaise(paymentAmount);
				player.AddChips(-paymentAmount);

				this->_pot += paymentAmount;
				this->_lastGameRaise = paymentAmount;

				isCorrect = true;
				currentCall = 0;
			}
		}

		currentPlayerIndex = (++currentPlayerIndex) % MAX_PLAYERS;

		activePlayers = ActivePlayersCount(players);
	}
}

void ClearDeal(Deal* deal)
{
	if (deal->_dealFlag)
	{
		deal->_pot = 0;
	}

	deal->_lastGameRaise = 0;
}
