#include <iostream>
#include <fstream>

#include "GlobalConstants.h"
#include "GlobalEnums.h"
#include "GamePlay.h"
#include "Player.h"
#include "Deal.h"

int ActivePlayersCount(Player* players);
void ActualizePlayers(Player*);
void GameInitPlayers(Player*, int);
void GameClear(Player*);
int GameSetPlayersNum();
FileCondition GameSaveToFile(Player* players);
GameCondition GameLoop(Player*);
FileCondition GameReadFromFile(Player*);
void GameChoisNewGame(Player*);

void ActualizePlayers(Player* players)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];

		if (player._playerActive != PlayerCondition::Unactive)
		{
			if (player._chips < CHIP_VALUE)
			{
				player._playerActive = PlayerCondition::Unactive;
			}
			else
			{
				player._playerActive = PlayerCondition::Active;
				player._lastRaice = 0;
			}
		}
	}
}

GameCondition GameLoop(Player* players)
{
	Deal* deal = new Deal();
	deal->_dealFlag = FIRST_DEAL;
	ClearDeal(deal);
	GameCondition condition = GameCondition::Continue;

	while (condition == GameCondition::Continue)
	{
		if (DealLoop(players, deal) == GameCondition::DealEnd)
		{
			ActualizePlayers(players);

			if (ActivePlayersCount(players) == 1)
			{
				condition = GameCondition::Win;
			}
			else
			{				
				std::cout << "Play again? y/n: ";
				char ch;
				std::cin >> ch;
				while (std::cin.get() != '\n');

				if (ch == 'n' || ch == 'N')
				{
					condition = GameCondition::End;
				}
			}
		}
	}

	delete deal;
	deal = nullptr;

	return condition;
}

FileCondition GameReadFromFile(Player* players)
{
	FileCondition result = FileCondition::Error;

	std::ifstream f(FILE_NAME);

	if (f.is_open())
	{
		int playerChips;

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			f >> playerChips;

			players[i]._chips = playerChips;
			players[i]._playerActive = PlayerCondition::Active;
		}

		result = FileCondition::OK;

		ActualizePlayers(players);
	}

	f.close();

	return result;
}

void GameChoisNewGame(Player* players)
{
	std::cout << "Game Continue" << std::endl << "New Game" << std::endl << "Choice c/n: ";

	char ch;
	std::cin >> ch;
	while (std::cin.get() != '\n');

	GameClear(players);
	FileCondition f = GameReadFromFile(players);

	bool choisFlag = (ch == 'c' || ch == 'C') && (f == FileCondition::OK);

	if (!choisFlag)
	{
		int playersNum = GameSetPlayersNum();
		GameInitPlayers(players, playersNum);
	}
}

void GameRun()
{
	Player* players = new Player[MAX_PLAYERS];

	GameCondition condition = GameCondition::Win;

	while (condition == GameCondition::Win)
	{
		GameChoisNewGame(players);

		condition = GameLoop(players);

		if (condition == GameCondition::End)
		{
			GameSaveToFile(players);
		}
	}

	delete[] players;
	players = nullptr;
}

int ActivePlayersInDealCount(Player* players)
{
	int activeCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		activeCount += IsPlayerInDeal(players[i]._playerActive) ? 1 : 0;
	}

	return activeCount;
}

int ActivePlayersCount(Player* players)
{
	int activeCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		activeCount += IsPlayerInGame(players[i]._playerActive) ? 1 : 0;
	}

	return activeCount;
}

void GameInitPlayers(Player* players, int playersNum)
{
	GameClear(players);

	for (int i = 0; i < playersNum; i++)
	{
		players[i]._chips = CHIP_VALUE * START_POINTS;
		players[i]._playerActive = PlayerCondition::Active;
	}
}

void GameClear(Player* players)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		InitEmptyPlayer(players[i]);
	}
}

int GameSetPlayersNum()
{
	bool isChoisCorect = false;

	int playersNum;

	while (!isChoisCorect)
	{
		std::cin.clear();

		std::cout << "How many players are going to play("
			<< MIN_PLAYERS
			<< " - "
			<< MAX_PLAYERS
			<< ")? ";

		if (!(std::cin >> playersNum))
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cout << std::endl << WARNING << std::endl;
			isChoisCorect = false;
		}
		else
		{
			bool correctNum = (playersNum >= MIN_PLAYERS && playersNum <= MAX_PLAYERS);
			if (correctNum)
			{
				isChoisCorect = true;
			}
			/*else
			{
				std::cout << std::endl << WARNING << std::endl;
				isChoisCorect = false;
			}*/
		}
	}

	return playersNum;
}

FileCondition GameSaveToFile(Player* players)
{
	FileCondition result = FileCondition::OK;

	std::ofstream f(FILE_NAME);

	try
	{
		if (f.is_open())
		{
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				Player& player = players[i];

				f << player._chips << " ";
			}
		}
	}
	catch (const std::exception&)
	{
		result = FileCondition::Error;
	}

	f.close();

	return result;
}

bool IsPlayerInGame(player_condition_type condition)
{
	return (condition != PlayerCondition::Unactive);
}