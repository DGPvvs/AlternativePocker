#include <iostream>

#include "Deal.h"
#include "GlobalConstants.h"
#include "GamePlay.h"

int CalcHalf(int pot)
{
	int half = pot / 2;
	if ((half % 10) == 5)
	{
		half += 5;
	}

	return half;
}

void DysplayPlayersInDeal(Player* players)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player._playerActive;

		if (IsPlayerInGame(condition))
		{
			std::cout << player._cardsAndRangeToString << std::endl << std::endl;
		}
	}
}

GameCondition DealLoop(Player* players, Deal* deal)
{
	int activeCount = ActivePlayersInDealCount(players);

	if (activeCount == 1)
	{
		return GameCondition::DealEnd;
	}

	if (DealStart(players, deal))
	{
		DealPlay(players, deal);
	}

	DeterminingWinner(players, deal);

	deal->_dealFlag = CONTINUE_DEAL;

	return DealLoop(players, deal);
}

bool DealStart(Player* players, Deal* deal)
{
	ClearDeal(deal);
	Card* cardDeck = new Card[DECK_COUNT];

	SetUpCardDesk(cardDeck);
	int currentDeskCount = DECK_COUNT;
	int* currentDeskCountPtr = &currentDeskCount;

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

			SetCards(player, cardDeck, currentDeskCount);
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
	int activePlayers = ActivePlayersInDealCount(players);

	while ((activePlayers > 1) && currentCall < activePlayers - 1)
	{
		deal->_currentMaxRaise = CalcMaxRaise(players);

		Player& player = players[currentPlayerIndex];
		player_condition_type condition = player._playerActive;
		if (IsPlayerInDeal(condition))
		{
			std::cout << "Pot: " << deal->_pot << std::endl << std::endl;

			bool isCorrect = false;

			while (!isCorrect)
			{
				player_condition_type ChoiceMade = PlayerCondition::Unactive;
				std::cout << "You have given: " << player._lastRaice / 10 << " chips" << std::endl;
				std::cout << "Last raise is: " << deal->_lastGameRaise / 10 << " chips" << std::endl << std::endl;
				std::cout << player._cardsAndRangeToString << std::endl;

				if (isFirst)
				{
					std::cout << player._name << " raise or fold? r/f: ";
					ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold;
				}
				else if (deal->_currentMaxRaise <= deal->_lastGameRaise)
				{
					std::cout << player._name << " call or fold? c/f: ";
					ChoiceMade = PlayerCondition::Fold | PlayerCondition::Call;
				}
				else
				{
					std::cout << player._name << " raise, call or fold? r/c/f: ";
					ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold | PlayerCondition::Call;
				}

				std::string s;
				std::getline(std::cin, s);
				std::cout << std::endl;

				if ((s == "f" || s == "F") && (ChoiceMade & PlayerCondition::Fold) == PlayerCondition::Fold)
				{
					player._playerActive = PlayerCondition::Fold;
					isCorrect = true;
				}
				else if ((s == "c" || s == "C") && !isFirst && (ChoiceMade & PlayerCondition::Call) == PlayerCondition::Call)
				{
					currentCall++;
					int lastPlayerRaise = player._lastRaice;
					int pays = deal->_lastGameRaise - lastPlayerRaise;

					player._lastRaice = deal->_lastGameRaise;

					player._chips -= pays;
					deal->_pot += pays;
					isCorrect = true;
				}
				else if ((s == "r" || s == "R") && (ChoiceMade & PlayerCondition::Raise) == PlayerCondition::Raise)
				{
					bool isCorrectPay = false;
					int paymentAmount;

					while (!isCorrectPay)
					{
						std::cout << player._name << " pay: (" << (deal->_lastGameRaise + CHIP_VALUE) / 10 << " - " << deal->_currentMaxRaise / 10 << " chips): ";
						std::getline(std::cin, s);
						std::cout << std::endl;

						try
						{
							paymentAmount = stoi(s) * 10;
							if ((paymentAmount >= deal->_lastGameRaise + CHIP_VALUE) && paymentAmount <= deal->_currentMaxRaise)
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

					player._lastRaice = paymentAmount;
					player._chips -= paymentAmount;

					deal->_pot += paymentAmount;
					deal->_lastGameRaise = paymentAmount;

					isCorrect = true;
					currentCall = 0;
				}
			}
		}

		currentPlayerIndex = (++currentPlayerIndex) % MAX_PLAYERS;

		activePlayers = ActivePlayersInDealCount(players);
	}
}

void DeterminingWinner(Player* players, Deal* deal)
{
	int maxPoint = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player._playerActive;

		if (IsPlayerInDeal(condition) && player._currentPoints > maxPoint)
		{
			maxPoint = player._currentPoints;
		}
	}

	int winnersCount = 0;
	int lastWinnerIdx = -1;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player._playerActive;
		if (IsPlayerInDeal(condition))
		{
			if (player._currentPoints != maxPoint)
			{
				player._playerActive = PlayerCondition::Hold;
			}
			else
			{
				winnersCount++;
				lastWinnerIdx = i;
			}
		}
	}

	if (winnersCount > 1)
	{
		int halfPot = CalcHalf(deal->_pot);

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			Player& player = players[i];
			player_condition_type condition = player._playerActive;

			if (condition == PlayerCondition::Active)
			{
				if (player._chips < CHIP_VALUE)
				{
					player._chips = CHIPS_ADD_VALUE * CHIP_VALUE;
				}
			}
			else if (condition == PlayerCondition::Hold)
			{
				if (player._chips < (halfPot + CHIP_VALUE))
				{
					player._playerActive = PlayerCondition::Fold;
				}
				else
				{
					bool isCorrect = false;

					while (!isCorrect)
					{
						std::cout << player._name << " will you pay half pot? y/n:";
						std::string s;
						std::getline(std::cin, s);
						std::cout << std::endl << std::endl;

						if (s == "y" || s == "Y")
						{
							player._chips -= halfPot;
							deal->_pot += halfPot;
							player._playerActive = PlayerCondition::Active;
							player._lastRaice = 0;
							isCorrect = true;
						}
						else if (s == "n" || s == "N")
						{
							player._playerActive = PlayerCondition::Fold;
							player._lastRaice = 0;
							isCorrect = true;
						}
					}
				}
			}
		}
	}
	else
	{
		Player& winner = players[lastWinnerIdx];

		winner._chips += deal->_pot;
		winner._lastRaice = 0;
		winner._playerActive = PlayerCondition::Active;
		deal->_pot = 0;
		deal->_lastGameRaise = 0;

		std::cout << winner._name << " is winner." << std::endl << std::endl;
		DysplayPlayersInDeal(players);
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