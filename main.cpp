#include "game.hpp"

int main(){
	setlocale(LC_ALL, "Russian");
	std::cout << "����� ���������� � ���� '��������-������'\n���������� ������������� ��� ������ ��������� ��������\n";
	while(startNewGame()){}
	return EXIT_SUCCESS;
}