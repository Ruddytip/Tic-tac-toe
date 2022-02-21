#include "game.hpp"

bool checkEndGame(std::string buffMap, std::string message){
	std::string question("Хотите сыграть ещё раз?(y/n)");
	std::string answer[2];
	std::string input;
	answer[0] = question[question.length() - 4];
	answer[1] = question[question.length() - 2];
	printMap(buffMap);
	std::cout << message << "\033[8C" << question;	
	getline(std::cin, input);
	std::cout << "\033[1A";
	while(!(input == answer[0] || input == answer[1])){
		std::cout << "\033[5A";
		printMap(buffMap);
		std::cout << "Введите корректные данные(" << answer[0] << "/" << answer[1] << ")";
		getline(std::cin, input);
	}
	std::cout << "\033[5A";
	return input == answer[0] ? true : false;
}

bool checkPlayer(){
	return questionTemplate("Ваш соперник компьютер или второй игрок?(c/p)");
}

bool checkSymbol(){
	return questionTemplate("Выберете символ, которым будете играть(x/o)");
}

bool checkTurn(){
	return questionTemplate("Желаете ходить первым?(y/n)");
}

bool questionTemplate(const std::string question){
	std::string answer[2];
	std::string input;
	answer[0] = question[question.length() - 4];
	answer[1] = question[question.length() - 2];
	printMap("         ");
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

bool startNewGame(){
	return (checkPlayer() ? logicPvE() : logicPvP());
}

int checkWin(const std::string buffMap){
	char symbol(' ');
	if(buffMap[0] == buffMap[1] && buffMap[0] == buffMap[2] && buffMap[0] != ' ') symbol = buffMap[0];
	if(buffMap[3] == buffMap[4] && buffMap[3] == buffMap[5] && buffMap[3] != ' ') symbol = buffMap[3];
	if(buffMap[6] == buffMap[7] && buffMap[6] == buffMap[8] && buffMap[6] != ' ') symbol = buffMap[6];

	if(buffMap[0] == buffMap[3] && buffMap[0] == buffMap[6] && buffMap[0] != ' ') symbol = buffMap[0];
	if(buffMap[1] == buffMap[4] && buffMap[1] == buffMap[7] && buffMap[1] != ' ') symbol = buffMap[1];
	if(buffMap[2] == buffMap[5] && buffMap[2] == buffMap[8] && buffMap[2] != ' ') symbol = buffMap[2];

	if(buffMap[0] == buffMap[4] && buffMap[0] == buffMap[8] && buffMap[0] != ' ') symbol = buffMap[0];
	if(buffMap[2] == buffMap[4] && buffMap[2] == buffMap[6] && buffMap[2] != ' ') symbol = buffMap[2];

	if(symbol == ' '){
		return 0;
	}else{
		return (symbol == 'x' ? 1 : 2);
	}
}

void calculateTurn(std::string* buffMap, const bool flagSymbol){
	char point = (flagSymbol == 1 ? 'o' : 'x');
	char pointEnemy = (flagSymbol == 1 ? 'x' : 'o');

	// Если компьютер может совершить победный ход, то он его делает
	for(int i = 0; i < 9; ++i){		
		if((*buffMap)[i] != ' ') continue;
		std::string buffMapTest(*buffMap);
		buffMapTest[i] = point;
		int win = checkWin(buffMapTest);
		if(win != 0) {
			if(win == 1 && point == 'x'){(*buffMap)[i] = point; return;}
			if(win == 2 && point == 'o'){(*buffMap)[i] = point; return;}
		}
	}

	// Если игрок на следующем ходу может победить, то компьютер блокирует ячейку
	for(int i = 0; i < 9; ++i){		
		if((*buffMap)[i] != ' ') continue;
		std::string buffMapTest = (*buffMap);
		buffMapTest[i] = pointEnemy;
		int win = checkWin(buffMapTest);
		if(win != 0) {
			if(win == 1 && point == 'o'){(*buffMap)[i] = point; return;}
			if(win == 2 && point == 'x'){(*buffMap)[i] = point; return;}
		}
	}

	// Компьютер занимает свободный угол при возможности
	if((*buffMap)[0] == ' ') {(*buffMap)[0] = point; return;}
	if((*buffMap)[2] == ' ') {(*buffMap)[2] = point; return;}
	if((*buffMap)[6] == ' ') {(*buffMap)[6] = point; return;}
	if((*buffMap)[8] == ' ') {(*buffMap)[8] = point; return;}

	// Компьютер занимает центр при возможности
	if((*buffMap)[4] == ' ') {(*buffMap)[4] = point; return;}

	// Компьютер занимает свободную сторону при возможности
	for(int i = 1; i < 8; i+=2)
		if((*buffMap)[i] == ' ') {(*buffMap)[i] = point; return;}
}

void inputTurn(std::string* buffMap, const bool flagSymbol){
	int x(0), y(0), error(-1);
		while(true){
			std::string errorName[3] = {
				" значения полей указанны не верно\n",
				" значения полей могут быть только в пределах от 1 до 3\n",
				" данное поле уже занято\n"};
			std::string move("");
			getline(std::cin, move);
			std::cout << (error >= 0 ? "\033[6A" : "\033[5A");
			printMap(*buffMap);
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
					if((*buffMap)[(y - 1) * 3 + (x - 1)] != ' ') error = 2;
				}
			}
			if(error >= 0){
				std::cout << "\r\033[8CОшибка ввода:" << errorName[error];
				std::cout << "\r\033[8CПовторите попытку ввода: ";
			} else{
				break;
			}
		}
		std::cout << "\033[4A";
		(*buffMap)[(y - 1) * 3 + (x - 1)] = (flagSymbol == 1 ? 'x' : 'o');
}

bool logicPvE(){	
	bool flagSymbol = checkSymbol();
	bool flagTurn = checkTurn();
	int flagWin = 0;
	std::string buffMap("         ");
	for(int i = 0; i < buffMap.length(); ++i){
		if(flagTurn != (i % 2)){
			printMap(buffMap);
			std::cout << "Ваш ход: ";
			inputTurn(&buffMap, flagSymbol);
		}else{
			calculateTurn(&buffMap, flagSymbol);
		}
		flagWin = checkWin(buffMap);
		if(flagWin != 0) break;
	}
	std::string message;
	switch (flagWin){
	case 0: message = "Ничья!!!\n"; break;
	case 1: message = (flagSymbol == 1 ? "Вы победили!!!\n" : "Вы Проиграли.\n"); break;
	case 2: message = (flagSymbol == 2 ? "Вы победили!!!\n" : "Вы Проиграли.\n"); break;
	}
	return checkEndGame(buffMap, message);
}

bool logicPvP(){
	int flagWin = 0;
	std::string buffMap = "         ";	
	for(int i = 0; i < buffMap.length(); ++i){
		printMap(buffMap);
		int flagSymbol = (i % 2 ? 0 : 1);	
		std::cout << "Ход "<< 2 - flagSymbol << " игрока: ";	
		inputTurn(&buffMap, flagSymbol);
		flagWin = checkWin(buffMap);
		if(flagWin != 0) break;
	}
	std::string message;
	switch (flagWin){
	case 0: message = "Ничья!!!\n"; break;
	case 1: message = "Победил первый игрок!!!\n"; break;
	case 2: message = "Победил второй игрок!!!\n"; break;
	}
	return checkEndGame(buffMap, message);
}

void printIntro(){
	std::cout << "        Добро пожальвать в игру 'Крестики-нолики'!\n";
	std::cout << "        Управление производиться при помощи ввода двух чисел,\n";
	std::cout << "        положение по оси 'x' и по оси 'y'\n";
	std::cout << "        Левое нижнее поле имеет номер '1:1', правое верхнее '3:3'\n";
}

void printMap(const std::string buffMap){
	clearScreen();
	printIntro();
	std::cout << "\033[4A";
	std::cout	<< "┌─┬─┬─┐\n"
				<< "│" << buffMap[6] << "│" << buffMap[7] << "│" << buffMap[8] << "│\n"  
				<< "├─┼─┼─┤\n"
				<< "│" << buffMap[3] << "│" << buffMap[4] << "│" << buffMap[5] << "│\n"  
				<< "├─┼─┼─┤\n"
				<< "│" << buffMap[0] << "│" << buffMap[1] << "│" << buffMap[2] << "│\n"  
				<< "└─┴─┴─┘";
	std::cout << "\033[2A\033[1C";
}

void clearScreen(){
	for(int i = 0; i < 7; ++i) {std::cout << "\033[K\n";}
	std::cout << "\033[7A";
}