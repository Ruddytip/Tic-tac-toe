#pragma once
#include "screen.hpp"

class CTictac{
private:
    char32_t map[3][3]; // Игровое поле
    bool firstTurn; // Флаг первого хода
    char32_t symbol, enemy; // Символы игрока и противника
    CScreen scr; // Виртуальный экран

    /*Шаблон для вопросов с выбором одного из двух предложенных вариантов,
    для корректной работы в конце строки question должна быть следующая конструкция:
    (a/b) - где 'a' и 'b' это разные латинские символы для вариантов ответа
    Возвращает true если игрок выбрал первый из предложенных символов*/
    bool questionTemplate(const std::u32string question);
    // Очистка игрового поля
    void clearMap();
    // Вывод на экран приветствие
    void printIntro();
    // Проверка на готовность играть
    // Возвращает True если игрок готов
    bool checkStart();
    // Начало новой игры
    // Возвращает True если игрок желает начать новую игру
    bool startNewGame();
    // Вывод на экран игровое поле
    void printMap();
    // Уточняет у игрока режим игры
    // Возвращает True если выбран режим PvE
    bool checkPlayer();
    // Запуск сценария двух игроков
    void playPVP();
    // Запуск сценария игры против компьютера
    void playPVE();
    // Обработка хода игрока
    // _flagTurn - флаг, показывающий чётный ли ход
    void inputTurn(bool _flagTurn);
    // Обработка хода компьютера
    void calculateTurn();
    // Проверка игрового поля на три символа в ряд
    // Возвращает символ победителя при нахождении комбинации,
    // а если таковых нет, то возвращает символ пробела
    char32_t checkWin();
    //
    std::string enterCord();
    int checkEnterErrors(std::string _data, bool _flagTurn);
public:
    CTictac();
    ~CTictac();
    void run(); // Функция старта игры
};