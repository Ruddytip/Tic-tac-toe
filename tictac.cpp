#include "tictac.hpp"

/*В программе используются Escape-код
Перемещение курсора вверх на N строк: \033[NA
Перемещение курсора вниз на N строк: \033[NB
Перемещение курсора вперед на N столбцов: \033[NC
Перемещение курсора назад на N столбцов: \033[ND
Стереть до конца строки: \033[K*/

CTictac::CTictac():
nWidth(3), nHeight(3){

}

CTictac::~CTictac(){

}

void CTictac::run(){
	while(startNewGame()){}
	std::cout << "\033[7B"; //Перемещение каретки курсора в самый низ игрового поля
}

bool CTictac::checkEndGame(std::string message){
	// Данная функция не использует шаблон вопросов, так как необходимо перед вопросом вывести результат прошлой игры
	std::string question("Хотите сыграть ещё раз?(y/n)");
	std::string answer[2];
	std::string input;
	answer[0] = question[question.length() - 4];
	answer[1] = question[question.length() - 2];
	printMap();
	std::cout << message << "\033[8C" << question;	
	getline(std::cin, input);
	std::cout << "\033[1A";
	while(!(input == answer[0] || input == answer[1])){
		std::cout << "\033[5A";
		printMap();
		std::cout << "Введите корректные данные(" << answer[0] << "/" << answer[1] << ")";
		getline(std::cin, input);
	}
	std::cout << "\033[5A";
	return input == answer[0] ? true : false;
}

bool CTictac::checkPlayer(){
	return questionTemplate("Ваш соперник компьютер или второй игрок?(c/p)");
}

bool CTictac::checkSymbol(){
	return questionTemplate("Выберете символ, которым будете играть(x/o)");
}

bool CTictac::checkTurn(){
	return questionTemplate("Желаете ходить первым?(y/n)");
}

bool CTictac::questionTemplate(const std::string question){
	std::string answer[2];
	std::string input;
	answer[0] = question[question.length() - 4];
	answer[1] = question[question.length() - 2];
	printMap();
	std::cout << question;	
	getline(std::cin, input);
	while(!(input == answer[0] || input == answer[1])){
		std::cout << "\033[1A";
		std::cout << "\033[8C\033[K";
		std::cout << "Введите корректные данные(" << answer[0] << "/" << answer[1] << ")";
		getline(std::cin, input);
	}
	std::cout << "\033[5A";
	return input == answer[0] ? true : false;
}

bool CTictac::startNewGame(){
	sFieldBuffer = std::string(nWidth * nHeight, Empty); // Очистка буфера поля
	return (checkPlayer() ? logicPvE() : logicPvP());
}

int CTictac::checkWin(std::string buffMap){
	char symbol(Empty);
	// Проверка строк
	if(buffMap[0] == buffMap[1] && buffMap[0] == buffMap[2] && buffMap[0] != Empty) symbol = buffMap[0];
	if(buffMap[3] == buffMap[4] && buffMap[3] == buffMap[5] && buffMap[3] != Empty) symbol = buffMap[3];
	if(buffMap[6] == buffMap[7] && buffMap[6] == buffMap[8] && buffMap[6] != Empty) symbol = buffMap[6];

	// Проверка столбцов
	if(buffMap[0] == buffMap[3] && buffMap[0] == buffMap[6] && buffMap[0] != Empty) symbol = buffMap[0];
	if(buffMap[1] == buffMap[4] && buffMap[1] == buffMap[7] && buffMap[1] != Empty) symbol = buffMap[1];
	if(buffMap[2] == buffMap[5] && buffMap[2] == buffMap[8] && buffMap[2] != Empty) symbol = buffMap[2];

	// Проверка диагоналей
	if(buffMap[0] == buffMap[4] && buffMap[0] == buffMap[8] && buffMap[0] != Empty) symbol = buffMap[0];
	if(buffMap[2] == buffMap[4] && buffMap[2] == buffMap[6] && buffMap[2] != Empty) symbol = buffMap[2];

	if(symbol == Empty){
		return 0;
	}else{
		return (symbol == Cross ? 1 : 2);
	}
}

void CTictac::calculateTurn(const bool flagSymbol){
	char point = (flagSymbol == 1 ? Zero : Cross);
	char pointEnemy = (flagSymbol == 1 ? Cross : Zero);

	// Если компьютер может совершить победный ход, то он его делает
	for(int i = 0; i < nWidth * nHeight; ++i){		
		if(sFieldBuffer[i] != Empty) continue;
		std::string buffMapTest = sFieldBuffer;
		buffMapTest[i] = point;
		int win = checkWin(buffMapTest);
		if(win != 0) {
			if(win == 1 && point == Cross){sFieldBuffer[i] = point; return;}
			if(win == 2 && point == Zero) {sFieldBuffer[i] = point; return;}
		}
	}

	// Если игрок на следующем ходу может победить, то компьютер блокирует ячейку
	for(int i = 0; i < nWidth * nHeight; ++i){		
		if(sFieldBuffer[i] != Empty) continue;
		std::string buffMapTest(sFieldBuffer);
		buffMapTest[i] = pointEnemy;
		int win = checkWin(buffMapTest);
		if(win != 0) {
			if(win == 1 && point == Zero){sFieldBuffer[i] = point; return;}
			if(win == 2 && point == Cross){sFieldBuffer[i] = point; return;}
		}
	}

	// Компьютер занимает свободный угол при возможности
	if(sFieldBuffer[0] == Empty) {sFieldBuffer[0] = point; return;}
	if(sFieldBuffer[2] == Empty) {sFieldBuffer[2] = point; return;}
	if(sFieldBuffer[6] == Empty) {sFieldBuffer[6] = point; return;}
	if(sFieldBuffer[8] == Empty) {sFieldBuffer[8] = point; return;}

	// Компьютер занимает центр при возможности
	if(sFieldBuffer[4] == Empty) {sFieldBuffer[4] = point; return;}

	// Компьютер занимает свободную сторону при возможности
	for(int i = 1; i < 8; i+=2)
		if(sFieldBuffer[i] == Empty) {sFieldBuffer[i] = point; return;}
}

void CTictac::inputTurn(const bool flagSymbol){
	int x(0), y(0), error(-1);
		while(true){ // Бесконечный цикл, пока игрок не введёт корректные данные
			std::string errorName[3] = { // Массив возможных ошибок
				" значения полей указанны не верно\n",
				" значения полей могут быть только в пределах от 1 до 3\n",
				" данное поле уже занято\n"};
			std::string move("");
			getline(std::cin, move);
			std::cout << (error >= 0 ? "\033[6A" : "\033[5A"); // Возврат каретки консоли в положение верхней левой ячейки игрового поля
			printMap();
			error = -1;
			if(move.length() != 3){
				error = 0;
			}else{
				std::stringstream iss(move);
				std::string dx(""), dy("");
				iss >> dx >> dy;
				x = std::atoi(dx.c_str());
				y = std::atoi(dy.c_str());
				if(x < 1 || x > 3 || y < 1 || y > 3){
					error = 1;
				}else{
					if(sFieldBuffer[(y - 1) * 3 + (x - 1)] != ' ') error = 2;
				}
			}
			if(error >= 0){
				std::cout << "\r\033[8CОшибка ввода:" << errorName[error];
				std::cout << "\r\033[8CПовторите попытку ввода: ";
			} else{
				break;
			}
		}
		std::cout << "\033[4A"; // Возврат каретки консоли в положение верхней левой ячейки игрового поля
		sFieldBuffer[(y - 1) * 3 + (x - 1)] = (flagSymbol == 1 ? 'x' : 'o');
}

bool CTictac::logicPvE(){	
	bool flagSymbol = checkSymbol();
	bool flagTurn = checkTurn();
	int flagWin = 0;
	for(int i = 0; i < sFieldBuffer.length(); ++i){
		if(flagTurn != (i % 2)){
			printMap();
			std::cout << "Ваш ход: ";
			inputTurn(flagSymbol);
		}else{
			calculateTurn(flagSymbol);
		}
		flagWin = checkWin(sFieldBuffer);
		if(flagWin != 0) break;
	}
	std::string message;
	switch (flagWin){
	case 0: message = "Ничья!!!\n"; break;
	case 1: message = (flagSymbol ? "Вы победили!!!\n" : "Вы Проиграли.\n"); break;
	case 2: message = (!flagSymbol ? "Вы победили!!!\n" : "Вы Проиграли.\n"); break;
	}
	return checkEndGame(message);
}

bool CTictac::logicPvP(){
	int flagWin = 0;
	for(int i = 0; i < sFieldBuffer.length(); ++i){
		printMap();
		int flagSymbol = (i % 2 ? 0 : 1);	
		std::cout << "Ход "<< 2 - flagSymbol << " игрока: ";	
		inputTurn(flagSymbol);
		flagWin = checkWin(sFieldBuffer);
		if(flagWin != 0) break;
	}
	std::string message;
	switch (flagWin){
	case 0: message = "Ничья!!!\n"; break;
	case 1: message = "Победил первый игрок!!!\n"; break;
	case 2: message = "Победил второй игрок!!!\n"; break;
	}
	return checkEndGame(message);
}

void CTictac::printIntro(){
	std::cout << "        Добро пожальвать в игру 'Крестики-нолики'!\n";
	std::cout << "        Управление производиться при помощи ввода двух чисел,\n";
	std::cout << "        положение по оси 'x' и по оси 'y'\n";
	std::cout << "        Левое нижнее поле имеет номер '1:1', правое верхнее '3:3'\n";
	std::cout << "\033[4A"; // Возврат каретки консоли в положение верхней левой ячейки игрового поля
}

void CTictac::printMap(){
	clearScreen();
	printIntro();
	std::cout	<< "┌─┬─┬─┐\n"
				<< "│" << sFieldBuffer[6] << "│" << sFieldBuffer[7] << "│" << sFieldBuffer[8] << "│\n"  
				<< "├─┼─┼─┤\n"
				<< "│" << sFieldBuffer[3] << "│" << sFieldBuffer[4] << "│" << sFieldBuffer[5] << "│\n"  
				<< "├─┼─┼─┤\n"
				<< "│" << sFieldBuffer[0] << "│" << sFieldBuffer[1] << "│" << sFieldBuffer[2] << "│\n"  
				<< "└─┴─┴─┘";
	std::cout << "\033[2A\033[1C"; // Коррекция положения каретки консоли, чтобы она оказалась справа от игрового поля и под всем остальным текстом
}

void CTictac::clearScreen(){
	for(int i = 0; i < nHeight * 2 + 1; ++i) {std::cout << "\033[K\n";} // Очистка всех строк, задействованных во время игры
	std::cout << "\033[7A"; // Возврат каретки консоли в положение верхней левой ячейки игрового поля
}