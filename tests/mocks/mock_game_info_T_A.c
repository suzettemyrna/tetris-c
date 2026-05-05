#include "../../src/core/include/game_info.h"
#include "../include/test_helpers.h"

int mock_resetGameInfo_calls = 0;
int mock_cleanGameInfo_calls = 0;
int mock_initGameInfo_calls = 0;


// ============================
// Внутренний объект для тестов

static char last_set_player[MAX_NAME_LENGTH + 1];

// =====
// Mocks

void initGameInfo()
{
    mock_initGameInfo_calls++;
}

void resetGameInfo()
{
    mock_resetGameInfo_calls++;
}

void cleanGameInfo()
{
    mock_cleanGameInfo_calls++;
}

GameInfo_t getCurrentGameInfo()
{
    GameInfo_t gi;
    memset(&gi, 0, sizeof(gi));
    gi.speed = 0;
    return gi;
}

void updateGameInfoPlayerName(const char *name) {
    strncpy(last_set_player, name, MAX_NAME_LENGTH + 1);
}