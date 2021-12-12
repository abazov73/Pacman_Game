#pragma once

#include "framework.h"

void DrawMap(HDC hdc);

void MoveGhosts();

void movePlayer();

void tryChangePlayerDirection();

void setNextDirection(int nextDirection);

void saveGame();

void loadGame();

void drawMenu(HDC hdc);

void savePlayerName(char* name);

void scanForPlayer();

void setGameMode(int gm);

void drawFinalScreen(HDC hdc, int msg);

void scanForVictory();

void returnToNormalState();

int loadLevel(int levelNum, HWND hWnd);