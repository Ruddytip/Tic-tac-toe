#include "game.hpp"

int main(){
	setlocale(LC_ALL, "Russian");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "Добро пожальвать в игру 'Крестики-нолики'\nУправление производиться при помощи латинских символов\n";
	while(startNewGame(&hConsole)){}
	return EXIT_SUCCESS;
}