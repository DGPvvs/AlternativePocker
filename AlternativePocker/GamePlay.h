#pragma once
#include "GlobalEnums.h"
#include "Card.h"
#include "Player.h"


void SetUpCardDesk(Card*);
void DeterminingWinner(Player*);

int ActivePlayersCount(Player*);
void ActualizePlayers(Player*);
void GameInitPlayers(Player*, int);

void GameClear(Player*);
int GameSetPlayersNum();
void GameSaveToFile(Player*);
GameCondition GameLoop(Player*);
FileCondition GameReadFromFile(Player*);
void GameChoisNewGame(Player*);
void GameRun();
