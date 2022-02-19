#include "game.hpp"

bool checkEndGame(){
	return questionTemplate("������ ������� ��� ���?(y/n)");
}

bool checkPlayer(){
	return questionTemplate("��� �������� ��������� ��� ������ �����?(c/p)");
}

bool checkSymbol(){
	return questionTemplate("�������� ������, ������� ������ ������(x/o)");
}

bool checkTurn(){
	return questionTemplate("������� ������ ������?(y/n)");
}

bool questionTemplate(const std::string question){
	std::string answer[2];
	std::string input;
	answer[0] = question[question.length() - 4];
	answer[1] = question[question.length() - 2];
	std::cout << question;	
	getline(std::cin, input);
	while(!(input == answer[0] || input == answer[1])){
		std::cout << "������� ���������� ������(" << answer[0] << "/" << answer[1] << ")";
		getline(std::cin, input);
	}
	return input == answer[0] ? true : false;
}

bool startNewGame(){
	system("cls");
	checkPlayer() ? logicPvE() : logicPvP();
	return checkEndGame();
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

	// ���� ��������� ����� ��������� �������� ���, �� �� ��� ������
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

	// ���� ����� �� ��������� ���� ����� ��������, �� ��������� ��������� ������
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

	// ��������� �������� ��������� ���� ��� �����������
	if((*buffMap)[0] == ' ') {(*buffMap)[0] = point; return;}
	if((*buffMap)[2] == ' ') {(*buffMap)[2] = point; return;}
	if((*buffMap)[6] == ' ') {(*buffMap)[6] = point; return;}
	if((*buffMap)[8] == ' ') {(*buffMap)[8] = point; return;}

	// ��������� �������� ����� ��� �����������
	if((*buffMap)[4] == ' ') {(*buffMap)[4] = point; return;}

	// ��������� �������� ��������� ������� ��� �����������
	for(int i = 1; i < 8; i+=2)
		if((*buffMap)[i] == ' ') {(*buffMap)[i] = point; return;}
}

void inputTurn(std::string* buffMap, const bool flagSymbol){
	int x(0), y(0);	
		std::cout << "����� ������ ���� ����� ����� '1;1', ������ ������� '3;3'\n";
		while(true){
			int error(-1);
			std::string errorName[3] = {
				" �������� ����� �������� �� �����\n",
				" �������� ����� ����� ���� ������ � �������� �� 1 �� 3\n",
				" ������ ���� ��� ������\n"};
			std::string move("");
			getline(std::cin, move);
			if(!move.length()) continue;
			std::cout << move.length();			
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
				printMap(*buffMap);
				std::cout << "������ ��� ����� ����� ������, ��������� �� ��� '�' � �� ��� '�'\n";
				std::cout << "����� ������� ���� ����� ����� '1;1', ������ ������ '3;3'\n";
				std::cout << "������ �����:" << errorName[error];
			} else{
				break;
			}
		}
		(*buffMap)[(y - 1) * 3 + (x - 1)] = (flagSymbol == 1 ? 'x' : 'o');
}

void logicPvE(){	
	bool flagSymbol = checkSymbol();
	bool flagTurn = checkTurn();
	int flagWin = 0;
	std::string buffMap = "         ";
	for(int i = 0; i < buffMap.length(); ++i){
		if(flagTurn != (i % 2)){
			printMap(buffMap);
			std::cout << "��� ���: ������� ��� ����� ����� ������, ��������� �� ��� '�' � �� ��� '�'\n";
			inputTurn(&buffMap, flagSymbol);
		}else{
			calculateTurn(&buffMap, flagSymbol);
		}
		flagWin = checkWin(buffMap);
		if(flagWin != 0) break;
	}
	printMap(buffMap);
	switch (flagWin){
	case 0: std::cout << "�����!!!\n"; break;
	case 1: std::cout << (flagSymbol == 1 ? "�� ��������!!!\n" : "�� ���������.\n"); break;
	case 2: std::cout << (flagSymbol == 2 ? "�� ��������!!!\n" : "�� ���������.\n"); break;
	}
}

void logicPvP(){
	int flagWin = 0;
	std::string buffMap = "         ";	
	for(int i = 0; i < buffMap.length(); ++i){
		printMap(buffMap);
		int flagSymbol = (i % 2 ? 0 : 1);	
		std::cout << "��� "<< 2 - flagSymbol << " ������: ������� ��� ����� ����� ������, ��������� �� ��� '�' � �� ��� '�'\n";	
		inputTurn(&buffMap, flagSymbol);
		flagWin = checkWin(buffMap);
		if(flagWin != 0) break;
	}
	printMap(buffMap);
	switch (flagWin){
	case 0: std::cout << "�����!!!\n"; break;
	case 1: std::cout << "������� ������ �����!!!\n"; break;
	case 2: std::cout << "������� ������ �����!!!\n"; break;
	}
}

void printMap(const std::string buffMap){
	std::string rows[3];
	for(int i = 0; i < 3; ++i){
		rows[i] = "| | | |\n";
		for(int j = 0; j < 3; ++j)
			rows[i][j * 2 + 1] = buffMap[i * 3 + j];
	}
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, {0,0});
	std::cout	<< "+-----+\n"
				<< rows[2]
				<< "|-+-+-|\n"
				<< rows[1]
				<< "|-+-+-|\n"
				<< rows[0]
				<< "+-----+\n";				
}