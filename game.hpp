#pragma once
#include <windows.h>
#include <sstream>
#include <iostream>

bool checkEndGame();
bool checkPlayer();
bool checkSymbol();
bool checkTurn();
bool questionTemplate(std::string question);
bool startNewGame(HANDLE* hnd);
void calculateTurn(HANDLE* hnd, std::string* buffMap, bool flagSymbol);
int checkWin(std::string buffMap);
void inputTurn(HANDLE* hnd, std::string* buffMap, bool flagSymbol);
void logicPvE(HANDLE* hnd, std::string* buffMap);
void logicPvP(HANDLE* hnd, std::string* buffMap);
void printMap(HANDLE* hnd, std::string buffMap);