#include "game.hpp"

int main(){
	while(startNewGame()){}
	std::cout << "\033[7B";
	return EXIT_SUCCESS;
}