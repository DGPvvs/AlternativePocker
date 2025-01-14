#pragma once
#include "GlobalEnums.h"
#include "Card.h"
#include "Player.h"


bool IsPlayerInGame(player_condition_type condition);
int ActivePlayersInDealCount(Player*);
void GameRun();