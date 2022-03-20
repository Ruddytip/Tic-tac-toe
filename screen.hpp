#pragma once
#include <iostream>
#include <vector>
#include <locale> // wstring_convert
#include <codecvt> // codecvt_utf8

struct COLOR{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

namespace SYMBOLS{
    const char32_t EMPTY  {' '};
    const char32_t CROSS  {'X'};
    const char32_t ZERO   {'O'};
    const char32_t ERROR  {'~'};
};

namespace COLORS{
    const COLOR WHITE   {255, 255, 255};
    const COLOR RED     {255, 0,   0};
    const COLOR GREEN   {0,   255, 0};
    const COLOR BLUE    {0,   0,   255};
    const COLOR YELLOW  {255, 255, 0};
    const COLOR MAGENTA {255, 0,   255};
    const COLOR CYAN    {0,   255, 255};
    const COLOR BLACK   {0,   0,   0};
};

struct VirtualPixel{ // Структара виртуального пикселя
    COLOR penColor; // Цвет символа
    COLOR brushColor; // Цвет фона
    char32_t symbol; // Символ
};

class CScreen{ // Клас экрана виртуального консоли
private:
    uint8_t width, height; // Ширина и высота экрана
    std::vector<VirtualPixel> pixels; // Массив всех виртуальных пикселей
    std::vector<std::string> screenBuffer; // Массив строк буфера экрана
    // Конвертирует значения виртуальных пикселей в буфер экрана
    void convertBuffer();
    // Очищает буфер экрана
    void clearScreenBuffer();
    // Конвертирует числовое значение цвета RGB в строку
    std::string convertColor(COLOR _colorPen, COLOR _colorBrush);
public:
    // Задаёт ширину и высоту экрана
    CScreen(uint8_t _width, uint8_t _height);
    ~CScreen();
    // Вывод на экран
    void show();
    // Очистка буфера
    void clear();
    // Установить в указанных координатах символ _symbol цвета _penColor на фоне _brushColor
    void setPixel(uint8_t _x, uint8_t _y, COLOR _penColor, COLOR _brushColor, char32_t _symbol);
    // возвращает цвет символа    
    COLOR getPenColor(uint8_t _x, uint8_t _y);
    // Возвращает цвет фона
    COLOR getBrushColor(uint8_t _x, uint8_t _y);
    // Возвращает символ
    char32_t getSymbol(uint8_t _x, uint8_t _y);
    // Вывод текста на экран
    void setText(int _x, int _y, COLOR _penColor, COLOR _brushColor, std::u32string _text);
};