#pragma once
#include <windows.h>
#include <sstream>
#include <iostream>

bool checkEndGame();
bool checkPlayer();
bool checkSymbol();
bool checkTurn();
bool questionTemplate(const std::string question);
bool startNewGame();
int checkWin(const std::string buffMap);
void calculateTurn(std::string* buffMap, const bool flagSymbol);
void inputTurn(std::string* buffMap, const bool flagSymbol);
void logicPvE();
void logicPvP();
void printMap(const std::string buffMap);