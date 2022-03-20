#include "tictac.hpp"

CTictac::CTictac():
nWidth(80), nHeight(8), firstTurn(true), symbol(SYMBOLS::EMPTY), scr(nWidth, nHeight){
	clearMap();
	printIntro();
}

CTictac::~CTictac(){}

void CTictac::clearMap(){
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			map[i][j] = SYMBOLS::EMPTY;
}

void CTictac::printIntro(){
	scr.setText(0, 0, COLORS::YELLOW, COLORS::BLACK, U"Добро пожальвать в игру 'Крестики-нолики'!");
	scr.setText(0, 1, COLORS::YELLOW, COLORS::BLACK, U"Управление производиться при помощи ввода двух чисел,");
	scr.setText(0, 2, COLORS::YELLOW, COLORS::BLACK, U"положение по оси 'x' и по оси 'y'");
	scr.setText(0, 3, COLORS::YELLOW, COLORS::BLACK, U"Левое нижнее поле имеет номер '1:1', правое верхнее '3:3");
	scr.show();
}

bool CTictac::checkStart(){
	return questionTemplate(U"Готовы начать игру?(y/n)");
}

bool CTictac::startNewGame(){
	clearMap();
	scr.clear();
	printMap();
	checkPlayer() ? playPVE() : playPVP();
	return questionTemplate(U"Начать новую игру?(y/n)");
}

void CTictac::printMap(){
	scr.setText(0, 0, COLORS::WHITE, COLORS::BLACK, U"┌─┬─┬─┐");
	scr.setText(0, 1, COLORS::WHITE, COLORS::BLACK, U"│ │ │ │");
	scr.setText(0, 2, COLORS::WHITE, COLORS::BLACK, U"├─┼─┼─┤");
	scr.setText(0, 3, COLORS::WHITE, COLORS::BLACK, U"│ │ │ │");
	scr.setText(0, 4, COLORS::WHITE, COLORS::BLACK, U"├─┼─┼─┤");
	scr.setText(0, 5, COLORS::WHITE, COLORS::BLACK, U"│ │ │ │");
	scr.setText(0, 6, COLORS::WHITE, COLORS::BLACK, U"└─┴─┴─┘");

	COLOR clr;
	clr = (symbol == map[0][2] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(1, 1, clr, COLORS::BLACK, map[0][2]);	
	clr = (symbol == map[1][2] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(3, 1, clr, COLORS::BLACK, map[1][2]);	
	clr = (symbol == map[2][2] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(5, 1, clr, COLORS::BLACK, map[2][2]);	
	clr = (symbol == map[0][1] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(1, 3, clr, COLORS::BLACK, map[0][1]);	
	clr = (symbol == map[1][1] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(3, 3, clr, COLORS::BLACK, map[1][1]);	
	clr = (symbol == map[2][1] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(5, 3, clr, COLORS::BLACK, map[2][1]);	
	clr = (symbol == map[0][0] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(1, 5, clr, COLORS::BLACK, map[0][0]);
	clr = (symbol == map[1][0] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(3, 5, clr, COLORS::BLACK, map[1][0]);
	clr = (symbol == map[2][0] ? COLORS::GREEN : COLORS::RED);
	scr.setPixel(5, 5, clr, COLORS::BLACK, map[2][0]);
}

bool CTictac::checkPlayer(){
	return questionTemplate(U"Ваш соперник компьютер или второй игрок?(c/p)");
}

void CTictac::playPVP(){
	char32_t winFlag;
	symbol = SYMBOLS::CROSS;
	for(int i = 0; i < 9; ++i){
		inputTurn(i%2 == 0);
		winFlag = checkWin();
		if(winFlag != SYMBOLS::EMPTY) break;
	}

	switch (winFlag){
	case SYMBOLS::EMPTY:
	scr.setText(8, 1, COLORS::GREEN, COLORS::BLACK, U"Ничья!!!"); break;
	case SYMBOLS::CROSS:
	scr.setText(8, 1, COLORS::YELLOW, COLORS::BLACK, U"Победил первый игрок!!!"); break;
	case SYMBOLS::ZERO:
	scr.setText(8, 1, COLORS::YELLOW, COLORS::BLACK, U"Победил второй игрок!!!"); break;
	}
}

void CTictac::playPVE(){
	firstTurn = questionTemplate(U"Желаете ходить первым?(y/n)");
	symbol = questionTemplate(U"Выберете символ, которым будете играть(x/o)") ? SYMBOLS::CROSS : SYMBOLS::ZERO;
	char32_t winFlag;
	for(int i = 0; i < 9; ++i){
		firstTurn ? inputTurn(i%2 == 0): calculateTurn();
		firstTurn = !firstTurn;
		scr.clear();
		printMap();
		scr.show();
		winFlag = checkWin();
		if(winFlag != SYMBOLS::EMPTY) break;
	}

	if(winFlag == SYMBOLS::EMPTY){
		scr.setText(8, 1, COLORS::GREEN, COLORS::BLACK, U"Ничья!!!");
	}else{
		if(winFlag == symbol){
			scr.setText(8, 1, COLORS::GREEN, COLORS::BLACK, U"Вы победили!!!");
		}else{
			scr.setText(8, 1, COLORS::RED,   COLORS::BLACK, U"Вы проиграли(");
		}
	}
}

void CTictac::inputTurn(bool flagTurn){
	int x(0), y(0), error(-1);
	while(true){ // Бесконечный цикл, пока игрок не введёт корректные данные
		std::u32string errorName[3] = { // Массив возможных ошибок
			U"Значения полей указанны не верно",
			U"Значения полей могут быть только в пределах от 1 до 3",
			U"Занное поле уже занято"};
		std::string move("");
		scr.clear();
		printMap();
		scr.setText(0, 7, COLORS::WHITE, COLORS::BLACK, U"Введите координаты ячейки: ");
		scr.show();
		std::cout << "\033[1A\r\033[28C";
		getline(std::cin, move);
		error = -1;
		if(move.length() != 3){
			error = 0;
		}else{
			std::stringstream iss(move);
			std::string dx(""), dy("");
			iss >> dx >> dy;
			x = std::atoi(dx.c_str()) - 1;
			y = std::atoi(dy.c_str()) - 1;
			if(x < 0 || x > 2 || y < 0 || y > 2){
				error = 1;
			}else{
				if(map[x][y] != SYMBOLS::EMPTY) error = 2;
			}
		}
		if(error >= 0){
			scr.clear();
			printMap();
			scr.setText(8, 2, COLORS::RED, COLORS::BLACK, U"Ошибка ввода:");
			scr.setText(8, 3, COLORS::RED, COLORS::BLACK, errorName[error]);
			scr.show();
		} else{
			break;
		}
	}

	map[x][y] = (flagTurn ? SYMBOLS::CROSS : SYMBOLS::ZERO);
	scr.clear();
	printMap();
	scr.show();
}

void CTictac::calculateTurn(){
	char32_t point = (symbol == SYMBOLS::ZERO ? SYMBOLS::CROSS : SYMBOLS::ZERO);

	// Если компьютер может совершить победный ход, то он его делает
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j){		
			if(map[i][j] != SYMBOLS::EMPTY) continue;
			map[i][j] = point;
			char32_t win = checkWin();
			if(win == point){
				return;
			}else{
				map[i][j] = SYMBOLS::EMPTY;
			}
	}

	// Если игрок на следующем ходу может победить, то компьютер блокирует ячейку
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j){		
			if(map[i][j] != SYMBOLS::EMPTY) continue;
			map[i][j] = symbol;
			char32_t win = checkWin();
			if(win == symbol){
				map[i][j] = point;
				return;
			}else{
				map[i][j] = SYMBOLS::EMPTY;
			}
	}

	// Компьютер занимает свободный угол при возможности
	if(map[0][0] == SYMBOLS::EMPTY) {map[0][0] = point; return;}
	if(map[0][2] == SYMBOLS::EMPTY) {map[0][2] = point; return;}
	if(map[2][0] == SYMBOLS::EMPTY) {map[2][0] = point; return;}
	if(map[2][2] == SYMBOLS::EMPTY) {map[2][2] = point; return;}

	// Компьютер занимает центр при возможности
	if(map[1][1] == SYMBOLS::EMPTY) {map[1][1] = point; return;}

	// Компьютер занимает свободную сторону при возможности
	if(map[1][0] == SYMBOLS::EMPTY) {map[0][0] = point; return;}
	if(map[2][1] == SYMBOLS::EMPTY) {map[2][1] = point; return;}
	if(map[1][2] == SYMBOLS::EMPTY) {map[1][2] = point; return;}
	if(map[0][1] == SYMBOLS::EMPTY) {map[0][1] = point; return;}
}

char32_t CTictac::checkWin(){
	// Проверка строк
	if(map[0][0] != SYMBOLS::EMPTY && map[0][0] == map[1][0] && map[0][0] == map[2][0]) return map[0][0];
	if(map[0][1] != SYMBOLS::EMPTY && map[0][1] == map[1][1] && map[0][1] == map[2][1]) return map[0][1];
	if(map[0][2] != SYMBOLS::EMPTY && map[0][2] == map[1][2] && map[0][2] == map[2][2]) return map[0][2];

	// Проверка столбцов
	if(map[0][0] != SYMBOLS::EMPTY && map[0][0] == map[0][1] && map[0][0] == map[0][2]) return map[0][0];
	if(map[1][0] != SYMBOLS::EMPTY && map[1][0] == map[1][1] && map[1][0] == map[1][2]) return map[1][0];
	if(map[2][0] != SYMBOLS::EMPTY && map[2][0] == map[2][1] && map[2][0] == map[2][2]) return map[2][0];

	// Проверка диагоналей
	if(map[0][0] != SYMBOLS::EMPTY && map[0][0] == map[1][1] && map[0][0] == map[2][2]) return map[0][0];
	if(map[0][2] != SYMBOLS::EMPTY && map[0][2] == map[1][1] && map[0][2] == map[2][0]) return map[0][2];

	return SYMBOLS::EMPTY;
}

void CTictac::run(){
	if(!checkStart()) return;
	while(startNewGame()){}
}

bool CTictac::questionTemplate(const std::u32string question){
	std::string answer[2];
	std::string input;
	std::u32string errorText = U"Неверный ввод. ";
	answer[0] = question[question.length() - 4];
	answer[1] = question[question.length() - 2];
	scr.setText(0, 7, COLORS::BLACK, COLORS::BLACK, std::u32string(nWidth, SYMBOLS::EMPTY));
	scr.setText(0, 7, COLORS::WHITE, COLORS::BLACK, question);
	scr.show();
	std::cout << "\033[1A\r\033[" + std::to_string(question.length()) + "C";
	getline(std::cin, input);
	while(!(input == answer[0] || input == answer[1])){
		scr.setText(0, 7, COLORS::BLACK, COLORS::BLACK, std::u32string(nWidth, SYMBOLS::EMPTY));
		scr.setText(0, 7, COLORS::WHITE, COLORS::BLACK, errorText + question);
		scr.show();
		std::cout << "\033[1A\r\033[" + std::to_string(question.length() + errorText.length()) + "C";
		getline(std::cin, input);
	}
	return input == answer[0] ? true : false;
}
