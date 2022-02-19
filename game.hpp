#pragma once
#include <sstream>
#include <iostream>

bool checkEndGame(std::string buffMap, std::string message);
bool checkPlayer();
bool checkSymbol();
bool checkTurn();
bool questionTemplate(const std::string question);
bool startNewGame();
int checkWin(const std::string buffMap);
void calculateTurn(std::string* buffMap, const bool flagSymbol);
void inputTurn(std::string* buffMap, const bool flagSymbol);
bool logicPvE();
bool logicPvP();
void printIntro();
void printMap(const std::string buffMap);
void clearScreen();