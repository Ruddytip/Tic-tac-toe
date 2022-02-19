#include "game.hpp"

int main(){
	setlocale(LC_ALL, "Russian");
	while(startNewGame()){}
	std::cout << "\033[6B";
	return EXIT_SUCCESS;
}