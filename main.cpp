#include "game.hpp"

int main(){
	setlocale(LC_ALL, "Russian");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "����� ���������� � ���� '��������-������'\n���������� ������������� ��� ������ ��������� ��������\n";
	while(startNewGame(&hConsole)){}
	return EXIT_SUCCESS;
}