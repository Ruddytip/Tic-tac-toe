#pragma once
#include <windows.h>
#include <sstream>
#include <iostream>

bool checkEndGame();
bool checkPlayer();
bool questionTemplate(std::string question);
bool startNewGame(HANDLE* hnd);
int checkWin(std::string buffMap);
void logicPvE(HANDLE* hnd, std::string buffMap);
void logicPvP(HANDLE* hnd, std::string buffMap);
void printMap(HANDLE* hnd, std::string buffMap);