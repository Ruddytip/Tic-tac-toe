#include "game.hpp"

int main(){
	while(startNewGame()){}
	std::cout << "\033[6B";
	return EXIT_SUCCESS;
}