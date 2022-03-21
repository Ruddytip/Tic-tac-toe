#pragma once
#include <sstream>
#include "screen.hpp"

class CTictac{
private:
    char32_t map[3][3];
    uint8_t nWidth, nHeight;
    bool firstTurn, currentMove;
    char32_t symbol;
    CScreen scr;

    // /*Шаблон для вопросов с выбором одного из двух предложенных вариантов,
    // для корректной работы в конце строки question должна быть следующая конструкция:
    // (a/b) - где 'a' и 'b' это разные латинские символы для вариантов ответа
    // Возвращает true если игрок выбрал первый из предложенных символов*/
    bool questionTemplate(const std::u32string question);
    void clearMap();
    void printIntro();
    bool checkStart();
    bool startNewGame();
    void printMap();
    bool checkPlayer();
    void playPVP();
    void playPVE();
    void inputTurn();
    void calculateTurn();
    char32_t checkWin();
public:
    CTictac();
    ~CTictac();
    void run(); // Функция старта игры
};