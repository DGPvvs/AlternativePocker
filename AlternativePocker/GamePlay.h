#pragma once
#include "GlobalEnums.h"
#include "Card.h"
#include "Player.h"


bool IsPlayerInGame(player_condition_type condition);
int ActivePlayersCount(Player* players);
int ActivePlayersInDealCount(Player*);
void ActualizePlayers(Player*);
void GameInitPlayers(Player*, int);

void GameClear(Player*);
int GameSetPlayersNum();
FileCondition GameSaveToFile(Player* players);
GameCondition GameLoop(Player*);
FileCondition GameReadFromFile(Player*);
void GameChoisNewGame(Player*);
void GameRun();