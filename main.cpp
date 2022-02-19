#include "game.hpp"

int main(){
	setlocale(LC_ALL, "ru_RU");
	while(startNewGame()){}
	std::cout << "\033[6B";
	return EXIT_SUCCESS;
}