#include <iostream>
#include <fstream>

#include "GlobalConstants.h"
#include "GlobalEnums.h"
#include "GamePlay.h"
#include "Player.h"
#include "Deal.h"

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
				std::string s;
				std::cout << "Play again? y/n: ";
				std::getline(std::cin, s);

				if (s == "n" || s == "N")
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
	FileCondition result = FileCondition::OK;
	GameClear(players);

	std::ifstream f(FILE_NAME);

	try
	{
		std::string s;
		while (getline(f, s))
		{
			int playersNum = stoi(s);

			for (int i = 0; i < playersNum; i++)
			{
				getline(f, s);
				std::string name = s;

				getline(f, s);
				int chips = stoi(s);

				getline(f, s);
				int id = stoi(s);

				players[id]._name = name;
				players[id]._chips = chips;
				players[id]._id = id;
				players[id]._playerActive = PlayerCondition::Active;
				players[id]._isHasSevenClubs = SEVEN_CLUBS_NOT_PRESENT;
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

void GameChoisNewGame(Player* players)
{
	std::string s;
	std::cout << "Game Continue" << std::endl << "New Game" << std::endl << "Choice c/n: ";
	std::getline(std::cin, s);

	FileCondition f = GameReadFromFile(players);

	bool choisFlag = (s == "c" || s == "C") && (f == FileCondition::OK);

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

int ActivePlayersCount(Player* players)
{
	int activeCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		activeCount += IsPlayerInDeal(players[i]._playerActive) ? 1 : 0;
	}

	return activeCount;
}

void GameInitPlayers(Player* players, int playersNum)
{
	GameClear(players);

	for (int i = 0; i < playersNum; i++)
	{
		int plaiersId = i + 1;
		std::string playerName = std::string().append("Player").append(std::to_string(plaiersId));

		players[i]._name = playerName;
		players[i]._chips = CHIP_VALUE * START_POINTS;
		players[i]._id = plaiersId;
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
		std::string s;

		std::cout << "How many players are going to play("
			<< MIN_PLAYERS
			<< " - "
			<< MAX_PLAYERS
			<< ")? ";

		std::getline(std::cin, s);

		try
		{
			playersNum = stoi(s);
			bool correctNum = (playersNum < MIN_PLAYERS || playersNum > MAX_PLAYERS);

			if (correctNum)
			{
				throw std::runtime_error("");
			}

			isChoisCorect = true;
		}
		catch (const std::exception&)
		{
			std::cout << WARNING << std::endl;
		}
	}

	return playersNum;
}
