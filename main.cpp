#include "game.hpp"

int main(){
	setlocale(LC_ALL, "Russian");
	std::cout << "Добро пожальвать в игру 'Крестики-нолики'\nУправление производиться при помощи латинских символов\n";
	while(startNewGame()){}
	return EXIT_SUCCESS;
}